# AutoGen Studio MCP Server

> **Model Context Protocol** server for seamless integration between LLMs (GitHub Copilot, Claude Desktop) and AutoGen Studio.

---

## ?? **What This Does**

Allows LLMs to **directly**:
- ? Execute prompts in AutoGen Studio
- ? Monitor session status in real-time
- ? Run builds (CMake + MSBuild)
- ? Save logs automatically
- ? List available teams

**No more**: Copy/paste prompts manually  
**Now**: Just ask Copilot to run tasks

---

## ?? **Quick Start**

### **1. Install Dependencies**

```bash
cd mcp-server
npm install
```

### **2. Build**

```bash
npm run build
```

### **3. Configure GitHub Copilot**

Add to your `~/.github-copilot/config.json`:

```json
{
  "mcpServers": {
    "autogen-studio": {
      "command": "node",
      "args": ["C:\\Users\\joaqu\\source\\repos\\Imagine Studio\\mcp-server\\dist\\index.js"],
      "env": {
        "PROJECT_ROOT": "C:\\Users\\joaqu\\source\\repos\\Imagine Studio"
      }
    }
  }
}
```

### **4. Configure Claude Desktop** (Alternative)

Add to `~/Library/Application Support/Claude/claude_desktop_config.json`:

```json
{
  "mcpServers": {
    "autogen-studio": {
      "command": "node",
      "args": ["C:\\Users\\joaqu\\source\\repos\\Imagine Studio\\mcp-server\\dist\\index.js"],
      "env": {
        "PROJECT_ROOT": "C:\\Users\\joaqu\\source\\repos\\Imagine Studio"
      }
    }
  }
}
```

### **5. Restart GitHub Copilot / Claude Desktop**

---

## ??? **Available Tools**

### **1. `execute_autogen_prompt`**

Execute a prompt in AutoGen Studio.

**Parameters**:
- `team_name` (required): Name of the team (e.g., "Imagine Studio DevTeam")
- `prompt` (required): The prompt text
- `task_id` (optional): Sprint task ID (e.g., "H1.1")

**Example in Copilot Chat**:
```
Execute AutoGen prompt for task H1.2:
Team: "Imagine Studio DevTeam"
Prompt: "Implement AssetDatabase.cpp following H1.2 specifications"
```

**Returns**:
```json
{
  "status": "completed",
  "output": "[Full agent conversation]",
  "duration": 210,
  "session_id": "abc123"
}
```

---

### **2. `list_autogen_teams`**

List all available teams in `autogen/teams/`.

**Example**:
```
List all AutoGen teams
```

**Returns**:
```json
{
  "teams": [
    {
      "name": "Imagine Studio DevTeam",
      "file": "team_complete.json",
      "type": "RoundRobinGroupChat",
      "agents": ["planner", "coder", "reviewer", "tester"]
    }
  ]
}
```

---

### **3. `get_autogen_session_status`**

Get real-time status of a running session.

**Parameters**:
- `session_id` (required): Session ID from `execute_autogen_prompt`

**Example**:
```
Get status of AutoGen session abc123
```

**Returns**:
```json
{
  "status": "running",
  "current_agent": "coder",
  "messages_count": 3,
  "elapsed_time": 45
}
```

---

### **4. `run_build`**

Execute CMake + MSBuild compilation.

**Parameters**:
- `configuration` (required): "Debug" or "Release"

**Example**:
```
Run Debug build
```

**Returns**:
```json
{
  "cmake": {
    "errors": 0,
    "warnings": 0,
    "output": "[CMake output]"
  },
  "msbuild": {
    "errors": 0,
    "warnings": 0,
    "output": "[MSBuild output]"
  }
}
```

---

### **5. `save_autogen_log`**

Save session log to `autogen/sessions/`.

**Parameters**:
- `task_id` (required): Task ID (e.g., "H1.1")
- `status` (required): "SUCCESS" | "FAILED" | "PARTIAL"
- `log_content` (required): Log text to save
- `duration` (optional): Execution time in seconds

**Example**:
```
Save AutoGen log for H1.1 with status SUCCESS
```

**Returns**:
```json
{
  "log_file": "autogen/sessions/2025-01-18_H1.1_success.md",
  "status": "saved"
}
```

---

## ?? **Usage Examples**

### **Execute Sprint Task Automatically**

**Before (Manual)**:
1. Open AutoGen Studio UI
2. Copy prompt from `autogen/prompts/sprint_v1.9.0/H1.1.md`
3. Paste in Playground
4. Click Run
5. Wait for completion
6. Copy output
7. Save log manually
8. Commit

**After (With MCP)**:

Just say in Copilot Chat:
```
Execute task H1.1 using AutoGen
```

Copilot will:
1. ? Read prompt from `autogen/prompts/sprint_v1.9.0/H1.1.md`
2. ? Execute in AutoGen Studio via MCP
3. ? Monitor until completion
4. ? Save log automatically
5. ? Return summary

---

### **Run Build and Report**

**Copilot Chat**:
```
Run Debug build and tell me if it passes
```

**MCP executes**:
```typescript
run_build({ configuration: "Debug" })
```

**Copilot responds**:
```
Build Results:
? CMake: 0 errors, 0 warnings
? MSBuild: 0 errors, 0 warnings

All checks passed!
```

---

### **Check AutoGen Session Status**

**Copilot Chat**:
```
What's the status of AutoGen session abc123?
```

**MCP executes**:
```typescript
get_autogen_session_status({ session_id: "abc123" })
```

**Copilot responds**:
```
Session Status:
- Status: Running
- Current Agent: coder
- Messages: 3/100
- Elapsed Time: 45 seconds
```

---

## ?? **Development**

### **Run in Dev Mode**

```bash
npm run dev
```

### **Build**

```bash
npm run build
```

### **Test Manually**

```bash
# Start MCP server
npm start

# In another terminal, test with MCP Inspector
npx @modelcontextprotocol/inspector node dist/index.js
```

---

## ?? **Architecture**

```
???????????????????
? GitHub Copilot  ?
?   or Claude     ?
???????????????????
         ? MCP Protocol (stdio)
         ?
???????????????????
?  MCP Server     ? ? This project
?  (TypeScript)   ?
???????????????????
         ? HTTP API
         ?
???????????????????
? AutoGen Studio  ?
? (localhost:8081)?
???????????????????
```

---

## ?? **Benefits**

| Before MCP | After MCP | Improvement |
|------------|-----------|-------------|
| Manual copy/paste | Automatic execution | ? 10x faster |
| No real-time status | Live monitoring | ?? Transparent |
| Manual log saving | Auto-saved logs | ??? Organized |
| Switch between tools | Single interface (Copilot) | ?? Seamless |

---

## ?? **Integration with Methodology**

The MCP server respects your sprint workflow:

1. **Reads** prompts from `autogen/prompts/sprint_vX.Y.Z/`
2. **Executes** via AutoGen Studio (4-agent workflow)
3. **Saves** logs to `autogen/sessions/`
4. **Updates** `docs/daily.md` (can be automated)
5. **Commits** changes (manual or automated)

---

## ?? **Requirements**

- Node.js 20+
- AutoGen Studio running (`http://127.0.0.1:8081`)
- GitHub Copilot Chat or Claude Desktop

---

## ?? **Troubleshooting**

### **MCP server not starting**

Check logs:
```bash
node dist/index.js
```

### **AutoGen Studio not responding**

Verify AutoGen Studio is running:
```bash
curl http://127.0.0.1:8081/api/health
```

### **Copilot can't find tools**

Restart Copilot after adding MCP configuration.

---

## ?? **Next Steps**

1. ? Install dependencies: `npm install`
2. ? Build: `npm run build`
3. ? Configure Copilot (see Quick Start)
4. ? Test: "List all AutoGen teams" in Copilot Chat
5. ? Execute first task: "Execute task H1.1 using AutoGen"

---

**Version**: 1.0.0  
**Author**: Joaquín Luct  
**License**: MIT  
**Project**: Imagine Studio Multi-Agent System
