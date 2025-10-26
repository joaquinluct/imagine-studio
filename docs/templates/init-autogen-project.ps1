# AutoGen Project Initialization Script
# Version: 1.0
# Purpose: Deploy AutoGen + MCP infrastructure in any new project

param(
    [Parameter(Mandatory=$true)]
    [string]$ProjectName,
    
    [Parameter(Mandatory=$true)]
    [ValidateSet("C++", "Python", "React", "Rust", "Unity", "Go", "Java", "Other")]
    [string]$ProjectType,
    
    [Parameter(Mandatory=$true)]
    [string]$BuildCommand,
    
    [Parameter(Mandatory=$false)]
    [string]$TestCommand = "",
    
    [Parameter(Mandatory=$false)]
    [string]$LintCommand = "",
    
    [Parameter(Mandatory=$false)]
    [string]$LanguageVersion = "",
    
    [Parameter(Mandatory=$false)]
    [ValidateSet("UTF-8", "UTF-8-BOM", "ASCII")]
    [string]$FileEncoding = "UTF-8",
    
    [Parameter(Mandatory=$false)]
    [ValidateSet("LF", "CRLF")]
    [string]$LineEndings = "LF",
    
    [Parameter(Mandatory=$false)]
    [string]$CodeStyle = ""
)

Write-Host "?? AutoGen Project Initialization" -ForegroundColor Cyan
Write-Host "=================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Project Name: $ProjectName" -ForegroundColor Green
Write-Host "Project Type: $ProjectType" -ForegroundColor Green
Write-Host "Build Command: $BuildCommand" -ForegroundColor Green
Write-Host ""

# Get script directory and project root
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = (Get-Location).Path

Write-Host "?? Creating directory structure..." -ForegroundColor Yellow

# Create directory structure
$Directories = @(
    "autogen/agents",
    "autogen/teams",
    "autogen/prompts/common",
    "autogen/sessions/summary",
    "autogen/outputs",
    "mcp-server/src",
    "docs/methodology",
    "docs/sprints"
)

foreach ($Dir in $Directories) {
    $FullPath = Join-Path $ProjectRoot $Dir
    if (-not (Test-Path $FullPath)) {
        New-Item -ItemType Directory -Path $FullPath -Force | Out-Null
        Write-Host "  ? Created: $Dir" -ForegroundColor Gray
    } else {
        Write-Host "  ??  Exists: $Dir" -ForegroundColor DarkGray
    }
}

Write-Host ""
Write-Host "?? Generating configuration files..." -ForegroundColor Yellow

# Define language-specific configurations
$LanguageConfigs = @{
    "C++" = @{
        LanguageVersion = "C++14"
        CodeStyle = "Google C++ Style Guide"
        FileExt = ".cpp, .h"
        Includes = "Project headers first, then system headers, alphabetically"
    }
    "Python" = @{
        LanguageVersion = "Python 3.11+"
        CodeStyle = "PEP 8"
        FileExt = ".py"
        Includes = "Standard lib, third-party, local imports"
    }
    "React" = @{
        LanguageVersion = "React 18 + TypeScript 5"
        CodeStyle = "ESLint + Prettier"
        FileExt = ".tsx, .ts"
        Includes = "React imports first, then libs, then local"
    }
    "Rust" = @{
        LanguageVersion = "Rust 1.70+"
        CodeStyle = "rustfmt + Clippy"
        FileExt = ".rs"
        Includes = "std, external crates, local modules"
    }
    "Unity" = @{
        LanguageVersion = "C# 9.0 (Unity 2022 LTS)"
        CodeStyle = "Unity conventions"
        FileExt = ".cs"
        Includes = "Unity namespaces first, then System, then local"
    }
}

$Config = $LanguageConfigs[$ProjectType]
if (-not $Config) {
    $Config = @{
        LanguageVersion = $LanguageVersion
        CodeStyle = $CodeStyle
        FileExt = "*"
        Includes = "N/A"
    }
}

# Override with user-provided values
if ($LanguageVersion) { $Config.LanguageVersion = $LanguageVersion }
if ($CodeStyle) { $Config.CodeStyle = $CodeStyle }

# Generate Planner Agent
$PlannerAgent = @"
{
  "component_type": "agent",
  "label": "Planner",
  "description": "Sprint Manager - Reads task details and creates implementation plans",
  "provider": "AssistantAgent",
  "version": "1.0.0",
  "config": {
    "name": "planner",
    "llm_config": {
      "config_list": [
        {
          "model": "gpt-4o"
        }
      ],
      "temperature": 0.1
    },
    "human_input_mode": "NEVER",
    "max_consecutive_auto_reply": 15,
    "system_message": "You are a Sprint Manager for $ProjectName ($ProjectType project).\n\nYour role:\n1. Read current task from docs/daily.md\n2. Read task details from docs/sprint_tasks.md\n3. Create clear, step-by-step implementation plan\n4. Break complex tasks into subtasks\n5. Identify files to create/modify\n\nAlways:\n- Use tools to read files (don't assume content)\n- Propose concrete file paths\n- Consider project structure in docs/MAIN.md\n- Say 'TASK_COMPLETED' when done\n\nNever:\n- Implement code (Coder's job)\n- Review code (Reviewer's job)\n- Run builds (Tester's job)"
  }
}
"@

# Generate Coder Agent
$CoderAgent = @"
{
  "component_type": "agent",
  "label": "Coder",
  "description": "Implementation Specialist - Writes clean, production-ready code",
  "provider": "AssistantAgent",
  "version": "1.0.0",
  "config": {
    "name": "coder",
    "llm_config": {
      "config_list": [
        {
          "model": "gpt-4o"
        }
      ],
      "temperature": 0.3
    },
    "human_input_mode": "NEVER",
    "max_consecutive_auto_reply": 20,
    "system_message": "You are an expert $ProjectType developer for $ProjectName.\n\nTechnical specs:\n- Language: $($Config.LanguageVersion)\n- Style: $($Config.CodeStyle)\n- File encoding: $FileEncoding\n- Line endings: $LineEndings\n- Include order: $($Config.Includes)\n\nYour role:\n1. Read Planner's implementation plan\n2. Read existing files if modifying\n3. Write clean, tested, production-ready code\n4. Follow project standards in docs/MAIN.md\n5. Use write_file tool to create/modify files\n6. Say 'CODE_READY' when implementation complete\n\nAlways:\n- Follow the exact style guide\n- Add clear comments\n- Handle errors properly\n- Write modular, reusable code\n\nNever:\n- Modify external libraries (external/, node_modules/, vendor/)\n- Skip error handling\n- Use deprecated APIs\n- Write temporary/placeholder code"
  }
}
"@

# Generate Reviewer Agent
$ReviewerAgent = @"
{
  "component_type": "agent",
  "label": "Reviewer",
  "description": "Quality Assurance - Reviews code against project standards",
  "provider": "AssistantAgent",
  "version": "1.0.0",
  "config": {
    "name": "reviewer",
    "llm_config": {
      "config_list": [
        {
          "model": "gpt-4o"
        }
      ],
      "temperature": 0.1
    },
    "human_input_mode": "NEVER",
    "max_consecutive_auto_reply": 10,
    "system_message": "You are a Code Reviewer for $ProjectName ($ProjectType project).\n\nReview checklist:\n1. Code follows $($Config.CodeStyle)\n2. File encoding is $FileEncoding\n3. Line endings are $LineEndings\n4. Include order: $($Config.Includes)\n5. No external library modifications\n6. Error handling present\n7. Comments are clear\n8. No security issues\n9. Performance considerations\n10. Follows DRY/SOLID principles\n\nYour role:\n1. Read files that Coder modified\n2. Check against standards in docs/MAIN.md\n3. Verify no external/ modifications\n4. Say 'REVIEW_APPROVED' if all OK\n5. Say 'REVIEW_REJECTED: [reason]' if issues found\n\nIf rejected:\n- List specific issues\n- Suggest fixes\n- Wait for Coder to re-implement\n\nNever:\n- Approve code with style violations\n- Skip security checks\n- Accept temporary solutions"
  }
}
"@

# Generate Tester Agent
$TesterAgent = @"
{
  "component_type": "agent",
  "label": "Tester",
  "description": "Build & Validation - Runs builds, tests, and linters",
  "provider": "AssistantAgent",
  "version": "1.0.0",
  "config": {
    "name": "tester",
    "llm_config": {
      "config_list": [
        {
          "model": "gpt-4o-mini"
        }
      ],
      "temperature": 0.1
    },
    "human_input_mode": "NEVER",
    "max_consecutive_auto_reply": 10,
    "system_message": "You are a Build & Test Engineer for $ProjectName ($ProjectType project).\n\nValidation steps:\n1. Run build: $BuildCommand\n2. Run tests: $TestCommand\n3. Run linter: $LintCommand\n\nYour role:\n1. Execute build command\n2. Parse output for errors/warnings\n3. Execute tests if build succeeds\n4. Execute linter if tests pass\n5. Say 'WORKFLOW_FINISHED_OK' if ALL pass (0 errors, 0 warnings)\n6. Say 'BUILD_FAILED: [reason]' if any step fails\n\nIf failed:\n- Show error details\n- Suggest fixes to Coder\n- Wait for re-implementation\n\nSuccess criteria:\n- Build: 0 errors, 0 warnings\n- Tests: All pass\n- Linter: No violations\n\nNever:\n- Accept warnings as OK\n- Skip test execution\n- Say WORKFLOW_FINISHED_OK if ANY step failed"
  }
}
"@

# Generate Team Configuration
$TeamConfig = @"
{
  "component_type": "team",
  "label": "$ProjectName DevTeam",
  "description": "Complete development workflow with 4 specialized agents",
  "provider": "RoundRobinGroupChat",
  "version": "1.0.0",
  "config": {
    "name": "${ProjectName}_devteam",
    "participants": [
      {
        "component_type": "agent",
        "component_ref": "planner"
      },
      {
        "component_type": "agent",
        "component_ref": "coder"
      },
      {
        "component_type": "agent",
        "component_ref": "reviewer"
      },
      {
        "component_type": "agent",
        "component_ref": "tester"
      }
    ],
    "admin_name": "admin",
    "messages": [],
    "max_round": 20,
    "speaker_selection_method": "round_robin",
    "allow_repeat_speaker": false
  },
  "termination": {
    "type": "text_mention",
    "text": "WORKFLOW_FINISHED_OK",
    "max_messages": 100
  }
}
"@

# Write agent files
$PlannerAgent | Out-File -FilePath (Join-Path $ProjectRoot "autogen/agents/planner_agent.json") -Encoding UTF8
$CoderAgent | Out-File -FilePath (Join-Path $ProjectRoot "autogen/agents/coder_agent.json") -Encoding UTF8
$ReviewerAgent | Out-File -FilePath (Join-Path $ProjectRoot "autogen/agents/reviewer_agent.json") -Encoding UTF8
$TesterAgent | Out-File -FilePath (Join-Path $ProjectRoot "autogen/agents/tester_agent.json") -Encoding UTF8
$TeamConfig | Out-File -FilePath (Join-Path $ProjectRoot "autogen/teams/team_complete.json") -Encoding UTF8

Write-Host "  ? Generated: planner_agent.json" -ForegroundColor Gray
Write-Host "  ? Generated: coder_agent.json" -ForegroundColor Gray
Write-Host "  ? Generated: reviewer_agent.json" -ForegroundColor Gray
Write-Host "  ? Generated: tester_agent.json" -ForegroundColor Gray
Write-Host "  ? Generated: team_complete.json" -ForegroundColor Gray

# Generate MCP Server package.json
$MCPPackageJson = @"
{
  "name": "$($ProjectName.ToLower())-autogen-mcp",
  "version": "1.0.0",
  "description": "Model Context Protocol server for AutoGen Studio integration - $ProjectName",
  "main": "dist/index.js",
  "type": "module",
  "scripts": {
    "build": "tsc",
    "start": "node dist/index.js",
    "dev": "tsx src/index.ts"
  },
  "keywords": ["mcp", "autogen", "llm", "copilot"],
  "author": "$env:USERNAME",
  "license": "MIT",
  "dependencies": {
    "@modelcontextprotocol/sdk": "^0.4.0"
  },
  "devDependencies": {
    "@types/node": "^20.0.0",
    "typescript": "^5.3.0",
    "tsx": "^4.7.0"
  }
}
"@

$MCPPackageJson | Out-File -FilePath (Join-Path $ProjectRoot "mcp-server/package.json") -Encoding UTF8
Write-Host "  ? Generated: mcp-server/package.json" -ForegroundColor Gray

# Generate MCP Server TypeScript config
$MCPTsConfig = @"
{
  "compilerOptions": {
    "target": "ES2022",
    "module": "Node16",
    "lib": ["ES2022"],
    "outDir": "./dist",
    "rootDir": "./src",
    "strict": true,
    "esModuleInterop": true,
    "skipLibCheck": true,
    "forceConsistentCasingInFileNames": true,
    "moduleResolution": "Node16",
    "resolveJsonModule": true
  },
  "include": ["src/**/*"],
  "exclude": ["node_modules", "dist"]
}
"@

$MCPTsConfig | Out-File -FilePath (Join-Path $ProjectRoot "mcp-server/tsconfig.json") -Encoding UTF8
Write-Host "  ? Generated: mcp-server/tsconfig.json" -ForegroundColor Gray

# Generate MCP Server source
$MCPServerSource = @"
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

const PROJECT_ROOT = process.env.PROJECT_ROOT || '$($ProjectRoot.Replace('\', '\\'))';

class AutoGenStudioMCP {
  private server: Server;

  constructor() {
    this.server = new Server({
      name: '$($ProjectName.ToLower())-autogen-mcp',
      version: '1.0.0',
    });

    this.setupToolHandlers();
    this.setupErrorHandling();
  }

  private setupToolHandlers() {
    this.server.setRequestHandler(ListToolsRequestSchema, async () => ({
      tools: [
        {
          name: 'list_autogen_teams',
          description: 'List all available AutoGen teams',
          inputSchema: { type: 'object', properties: {} },
        },
        {
          name: 'run_build',
          description: 'Execute project build',
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
          description: 'Save AutoGen session log',
          inputSchema: {
            type: 'object',
            properties: {
              task_id: { type: 'string', description: 'Task ID' },
              status: {
                type: 'string',
                enum: ['SUCCESS', 'FAILED', 'PARTIAL'],
                description: 'Execution status',
              },
              log_content: { type: 'string', description: 'Log content' },
              duration: { type: 'number', description: 'Duration in seconds' },
            },
            required: ['task_id', 'status', 'log_content'],
          },
        },
      ],
    }));

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
            throw new Error(\`Unknown tool: \${request.params.name}\`);
        }
      } catch (error: any) {
        return {
          content: [{ type: 'text', text: \`Error: \${error.message}\` }],
          isError: true,
        };
      }
    });
  }

  private async listTeams() {
    const teamsDir = path.join(PROJECT_ROOT, 'autogen', 'teams');
    const files = await fs.readdir(teamsDir);
    const teams = [];

    for (const file of files) {
      if (file.endsWith('.json')) {
        const content = await fs.readFile(path.join(teamsDir, file), 'utf-8');
        const teamData = JSON.parse(content);
        teams.push({
          name: teamData.label || file.replace('.json', ''),
          file,
          type: teamData.provider,
          agents: teamData.config?.participants?.map((p: any) => p.component_ref) || [],
        });
      }
    }

    return {
      content: [{ type: 'text', text: JSON.stringify({ teams }, null, 2) }],
    };
  }

  private async runBuild(args: any) {
    const { configuration } = args;

    try {
      const result = await execAsync('$BuildCommand', {
        cwd: PROJECT_ROOT,
        maxBuffer: 1024 * 1024 * 50,
      });

      return {
        content: [
          {
            type: 'text',
            text: \`Build Result:\n\nStatus: SUCCESS ?\n\nOutput:\n\${result.stdout}\`,
          },
        ],
      };
    } catch (error: any) {
      return {
        content: [
          {
            type: 'text',
            text: \`Build FAILED ?\n\nError: \${error.message}\`,
          },
        ],
      };
    }
  }

  private async saveLog(args: any) {
    const { task_id, status, log_content, duration = 0 } = args;
    const date = new Date().toISOString().split('T')[0];
    const logFile = path.join(
      PROJECT_ROOT,
      'autogen',
      'sessions',
      \`\${date}_\${task_id}_\${status.toLowerCase()}.md\`
    );

    const logTemplate = \`# AutoGen Session Log - \${task_id}

**Date**: \${new Date().toISOString()}
**Task**: \${task_id}
**Status**: \${status === 'SUCCESS' ? '?' : '?'} \${status}
**Duration**: \${duration}s

---

\${log_content}

---

**Logged by**: $ProjectName MCP Server
\`;

    await fs.writeFile(logFile, logTemplate, 'utf-8');

    return {
      content: [
        {
          type: 'text',
          text: JSON.stringify({ log_file: logFile, status: 'saved' }, null, 2),
        },
      ],
    };
  }

  private setupErrorHandling() {
    this.server.onerror = (error) => console.error('[MCP Error]', error);
    process.on('SIGINT', async () => {
      await this.server.close();
      process.exit(0);
    });
  }

  async run() {
    const transport = new StdioServerTransport();
    await this.server.connect(transport);
    console.error('$ProjectName MCP server running on stdio');
  }
}

const server = new AutoGenStudioMCP();
server.run().catch(console.error);
"@

$MCPServerSource | Out-File -FilePath (Join-Path $ProjectRoot "mcp-server/src/index.ts") -Encoding UTF8
Write-Host "  ? Generated: mcp-server/src/index.ts" -ForegroundColor Gray

Write-Host ""
Write-Host "?? Installing MCP Server dependencies..." -ForegroundColor Yellow

Push-Location (Join-Path $ProjectRoot "mcp-server")
try {
    npm install 2>&1 | Out-Null
    Write-Host "  ? npm install completed" -ForegroundColor Gray
    
    npm run build 2>&1 | Out-Null
    Write-Host "  ? MCP Server compiled" -ForegroundColor Gray
} catch {
    Write-Host "  ??  npm install/build failed - run manually" -ForegroundColor Yellow
}
Pop-Location

Write-Host ""
Write-Host "? AutoGen infrastructure deployed!" -ForegroundColor Green
Write-Host ""
Write-Host "?? Next steps:" -ForegroundColor Cyan
Write-Host "  1. Import agents in AutoGen Studio (http://localhost:8081)" -ForegroundColor White
Write-Host "  2. Add MCP config to ~/.github-copilot/config.json" -ForegroundColor White
Write-Host "  3. Use GitHub Copilot to generate sprint files" -ForegroundColor White
Write-Host ""
Write-Host "?? See: docs/templates/AUTOGEN_TEMPLATE_SETUP.md" -ForegroundColor Gray
