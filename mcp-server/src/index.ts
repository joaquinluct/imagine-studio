#!/usr/bin/env node

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
  CallToolRequestSchema,
  ListToolsRequestSchema,
} from '@modelcontextprotocol/sdk/types.js';
import axios from 'axios';
import { exec } from 'child_process';
import { promisify } from 'util';
import * as fs from 'fs/promises';
import * as path from 'path';

const execAsync = promisify(exec);

// AutoGen Studio API configuration
const AUTOGEN_API_URL = 'http://127.0.0.1:8081/api';
const PROJECT_ROOT = process.env.PROJECT_ROOT || 'C:\\Users\\joaqu\\source\\repos\\Imagine Studio';

/**
 * MCP Server for AutoGen Studio Integration
 */
class AutoGenStudioMCP {
  private server: Server;

  constructor() {
    this.server = new Server(
      {
        name: 'autogen-studio-mcp',
        version: '1.0.0',
      },
      {
        capabilities: {
          tools: {},
        },
      }
    );

    this.setupToolHandlers();
    this.setupErrorHandling();
  }

  private setupToolHandlers() {
    // List available tools
    this.server.setRequestHandler(ListToolsRequestSchema, async () => ({
      tools: [
        {
          name: 'execute_autogen_prompt',
          description: 'Execute a prompt in AutoGen Studio with a specific team',
          inputSchema: {
            type: 'object',
            properties: {
              team_name: {
                type: 'string',
                description: 'Name of the AutoGen team to use',
              },
              prompt: {
                type: 'string',
                description: 'The prompt to execute',
              },
              task_id: {
                type: 'string',
                description: 'Sprint task ID (e.g., H1.1)',
              },
            },
            required: ['team_name', 'prompt'],
          },
        },
        {
          name: 'list_autogen_teams',
          description: 'List all available AutoGen teams',
          inputSchema: {
            type: 'object',
            properties: {},
          },
        },
        {
          name: 'get_autogen_session_status',
          description: 'Get status of a running AutoGen session',
          inputSchema: {
            type: 'object',
            properties: {
              session_id: {
                type: 'string',
                description: 'Session ID to query',
              },
            },
            required: ['session_id'],
          },
        },
        {
          name: 'run_build',
          description: 'Execute CMake + MSBuild compilation',
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
          description: 'Save AutoGen session log to file',
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
          case 'execute_autogen_prompt':
            return await this.executePrompt(request.params.arguments);
          case 'list_autogen_teams':
            return await this.listTeams();
          case 'get_autogen_session_status':
            return await this.getSessionStatus(request.params.arguments);
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

  private async executePrompt(args: any) {
    const { team_name, prompt, task_id } = args;

    try {
      // Call AutoGen Studio API to execute prompt
      const response = await axios.post(`${AUTOGEN_API_URL}/sessions`, {
        team: team_name,
        prompt: prompt,
      });

      const sessionId = response.data.session_id;

      // Poll for completion
      const result = await this.pollSession(sessionId);

      // Save log automatically
      if (task_id) {
        await this.saveLog({
          task_id,
          status: result.status === 'completed' ? 'SUCCESS' : 'FAILED',
          log_content: result.output,
          duration: result.duration,
        });
      }

      return {
        content: [
          {
            type: 'text',
            text: JSON.stringify({
              status: result.status,
              output: result.output,
              duration: result.duration,
              session_id: sessionId,
            }, null, 2),
          },
        ],
      };
    } catch (error: any) {
      throw new Error(`Failed to execute prompt: ${error.message}`);
    }
  }

  private async pollSession(sessionId: string, maxAttempts = 60): Promise<any> {
    for (let i = 0; i < maxAttempts; i++) {
      const response = await axios.get(`${AUTOGEN_API_URL}/sessions/${sessionId}`);
      
      if (response.data.status === 'completed' || response.data.status === 'failed') {
        return response.data;
      }

      await new Promise(resolve => setTimeout(resolve, 5000)); // Wait 5 seconds
    }

    throw new Error('Session timeout');
  }

  private async listTeams() {
    try {
      // Read teams from filesystem
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

  private async getSessionStatus(args: any) {
    const { session_id } = args;

    try {
      const response = await axios.get(`${AUTOGEN_API_URL}/sessions/${session_id}`);
      
      return {
        content: [
          {
            type: 'text',
            text: JSON.stringify(response.data, null, 2),
          },
        ],
      };
    } catch (error: any) {
      throw new Error(`Failed to get session status: ${error.message}`);
    }
  }

  private async runBuild(args: any) {
    const { configuration } = args;

    try {
      // Run CMake build
      const cmakeResult = await execAsync(
        `cmake --build build --config ${configuration}`,
        { cwd: PROJECT_ROOT }
      );

      // Run MSBuild
      const msbuildResult = await execAsync(
        `msbuild "Imagine Studio.sln" /t:Build /p:Configuration=${configuration} /p:Platform=x64 /m /nologo`,
        { cwd: PROJECT_ROOT }
      );

      const parseErrors = (output: string) => {
        const errors = (output.match(/error/gi) || []).length;
        const warnings = (output.match(/warning/gi) || []).length;
        return { errors, warnings };
      };

      return {
        content: [
          {
            type: 'text',
            text: JSON.stringify({
              cmake: {
                ...parseErrors(cmakeResult.stdout),
                output: cmakeResult.stdout,
              },
              msbuild: {
                ...parseErrors(msbuildResult.stdout),
                output: msbuildResult.stdout,
              },
            }, null, 2),
          },
        ],
      };
    } catch (error: any) {
      throw new Error(`Build failed: ${error.message}`);
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
**Status**: ${status === 'SUCCESS' ? '?' : '?'} ${status}
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
