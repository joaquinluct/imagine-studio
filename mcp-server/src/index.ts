#!/usr/bin/env node

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
  CallToolRequestSchema,
  ListToolsRequestSchema,
} from '@modelcontextprotocol/sdk/types.js';
import { exec } from 'child_process';
import { promisify } from 'util';
import * as fs from 'fs/promises';
import * as path from 'path';

const execAsync = promisify(exec);

// Configuration
const PROJECT_ROOT = process.env.PROJECT_ROOT || 'C:\\Users\\joaqu\\source\\repos\\Imagine Studio';

/**
 * Simplified MCP Server for AutoGen Studio
 * Provides tools for build, team listing, and log management
 * Prompt execution must be done through AutoGen Studio UI
 */
class AutoGenStudioMCP {
  private server: Server;

  constructor() {
    this.server = new Server({
      name: 'autogen-studio-mcp',
      version: '1.0.0',
    });

    this.setupToolHandlers();
    this.setupErrorHandling();
  }

  private setupToolHandlers() {
    // List available tools
    this.server.setRequestHandler(ListToolsRequestSchema, async () => ({
      tools: [
        {
          name: 'list_autogen_teams',
          description: 'List all available AutoGen teams from autogen/teams/ directory',
          inputSchema: {
            type: 'object',
            properties: {},
          },
        },
        {
          name: 'run_build',
          description: 'Execute MSBuild compilation (CMake disabled)',
          inputSchema: {
            type: 'object',
            properties: {
              configuration: {
                type: 'string',
                enum: ['Debug', 'Release'],
                description: 'Build configuration',
              },
            },
            required: ['configuration'],
          },
        },
        {
          name: 'save_autogen_log',
          description: 'Save AutoGen session log to autogen/sessions/',
          inputSchema: {
            type: 'object',
            properties: {
              task_id: {
                type: 'string',
                description: 'Task ID (e.g., H1.1)',
              },
              status: {
                type: 'string',
                enum: ['SUCCESS', 'FAILED', 'PARTIAL'],
                description: 'Execution status',
              },
              log_content: {
                type: 'string',
                description: 'Log content to save',
              },
              duration: {
                type: 'number',
                description: 'Execution duration in seconds',
              },
            },
            required: ['task_id', 'status', 'log_content'],
          },
        },
      ],
    }));

    // Handle tool execution
    this.server.setRequestHandler(CallToolRequestSchema, async (request) => {
      try {
        switch (request.params.name) {
          case 'list_autogen_teams':
            return await this.listTeams();
          case 'run_build':
            return await this.runBuild(request.params.arguments);
          case 'save_autogen_log':
            return await this.saveLog(request.params.arguments);
          default:
            throw new Error(`Unknown tool: ${request.params.name}`);
        }
      } catch (error: any) {
        return {
          content: [
            {
              type: 'text',
              text: `Error: ${error.message}`,
            },
          ],
          isError: true,
        };
      }
    });
  }

  private async listTeams() {
    try {
      const teamsDir = path.join(PROJECT_ROOT, 'autogen', 'teams');
      const files = await fs.readdir(teamsDir);
      
      const teams = [];
      for (const file of files) {
        if (file.endsWith('.json')) {
          const content = await fs.readFile(path.join(teamsDir, file), 'utf-8');
          const teamData = JSON.parse(content);
          teams.push({
            name: teamData.label || file.replace('.json', ''),
            file: file,
            type: teamData.provider,
            agents: teamData.config?.participants?.map((p: any) => p.component_ref) || [],
          });
        }
      }

      return {
        content: [
          {
            type: 'text',
            text: JSON.stringify({ teams }, null, 2),
          },
        ],
      };
    } catch (error: any) {
      throw new Error(`Failed to list teams: ${error.message}`);
    }
  }

  private async runBuild(args: any) {
    const { configuration } = args;

    try {
      // Get MSBuild path
      const msbuildPath = process.env.MSBUILD_PATH || 'C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\MSBuild\\Current\\Bin';
      const msbuildExe = path.join(msbuildPath, 'msbuild.exe');

      // Run MSBuild only
      const msbuildResult = await execAsync(
        `"${msbuildExe}" "Imagine Studio.sln" /t:Build /p:Configuration=${configuration} /p:Platform=x64 /m /nologo`,
        { cwd: PROJECT_ROOT, maxBuffer: 1024 * 1024 * 50 }
      );

      const parseErrors = (output: string) => {
        const errorMatches = output.match(/(\d+) Error\(s\)/i);
        const warningMatches = output.match(/(\d+) Warning\(s\)/i);
        return {
          errors: errorMatches ? parseInt(errorMatches[1]) : 0,
          warnings: warningMatches ? parseInt(warningMatches[1]) : 0,
        };
      };

      const buildStats = parseErrors(msbuildResult.stdout + msbuildResult.stderr);
      const fullOutput = msbuildResult.stdout + msbuildResult.stderr;
      
      // Extract only last 100 lines for summary
      const lines = fullOutput.split('\n');
      const summary = lines.slice(-100).join('\n');

      return {
        content: [
          {
            type: 'text',
            text: `MSBuild Result:\n\nErrors: ${buildStats.errors}\nWarnings: ${buildStats.warnings}\n\nStatus: ${buildStats.errors === 0 ? 'SUCCESS ✅' : 'FAILED ❌'}\n\nLast 100 lines:\n${summary}`,
          },
        ],
      };
    } catch (error: any) {
      // Build failed with error
      const errorMessage = error.message || 'Unknown error';
      return {
        content: [
          {
            type: 'text',
            text: `Build FAILED ❌\n\nError: ${errorMessage}\n\nThis usually means there are compilation errors.`,
          },
        ],
      };
    }
  }

  private async saveLog(args: any) {
    const { task_id, status, log_content, duration = 0 } = args;

    try {
      const date = new Date().toISOString().split('T')[0];
      const logFile = path.join(
        PROJECT_ROOT,
        'autogen',
        'sessions',
        `${date}_${task_id}_${status.toLowerCase()}.md`
      );

      const logTemplate = `# AutoGen Session Log - ${task_id}

**Date**: ${new Date().toISOString()}
**Task**: ${task_id}
**Status**: ${status === 'SUCCESS' ? '✅' : '❌'} ${status}
**Duration**: ${duration}s

---

## Agent Execution Log

${log_content}

---

**Logged by**: autogen-studio-mcp
**Version**: 1.0
`;

      await fs.writeFile(logFile, logTemplate, 'utf-8');

      return {
        content: [
          {
            type: 'text',
            text: JSON.stringify({
              log_file: logFile,
              status: 'saved',
            }, null, 2),
          },
        ],
      };
    } catch (error: any) {
      throw new Error(`Failed to save log: ${error.message}`);
    }
  }

  private setupErrorHandling() {
    this.server.onerror = (error) => {
      console.error('[MCP Error]', error);
    };

    process.on('SIGINT', async () => {
      await this.server.close();
      process.exit(0);
    });
  }

  async run() {
    const transport = new StdioServerTransport();
    await this.server.connect(transport);
    console.error('AutoGen Studio MCP server running on stdio');
  }
}

// Start server
const server = new AutoGenStudioMCP();
server.run().catch(console.error);
