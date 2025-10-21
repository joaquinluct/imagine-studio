# Quick Setup - AutoGen Studio MCP

## ?? Installation (5 minutes)

### **Step 1: Install Node.js Dependencies**

```powershell
cd mcp-server
npm install
npm run build
```

### **Step 2: Verify AutoGen Studio is Running**

```powershell
# Should return 200 OK
curl http://127.0.0.1:8081/api/health
```

If not running:
```powershell
cd "C:\Users\joaqu\source\repos\Imagine Studio"
autogenstudio ui --port 8081 --appdir .
```

### **Step 3: Configure GitHub Copilot**

**File**: `~/.github-copilot/config.json` (create if doesn't exist)

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

**Important**: Replace path with your actual project path!

### **Step 4: Restart GitHub Copilot**

1. Close VS Code
2. Reopen VS Code
3. Wait for Copilot to load

### **Step 5: Test**

Open Copilot Chat and type:
```
List all AutoGen teams
```

**Expected response**:
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

## ? **Verification Checklist**

- [ ] Node.js installed (v20+)
- [ ] `npm install` completed
- [ ] `npm run build` successful
- [ ] AutoGen Studio running on port 8081
- [ ] MCP config added to `~/.github-copilot/config.json`
- [ ] VS Code restarted
- [ ] "List all AutoGen teams" works in Copilot Chat

---

## ?? **First Real Test**

Once verified, try:

```
Execute task H1.2 using AutoGen with team "Imagine Studio DevTeam"
```

Copilot will:
1. Read prompt from `autogen/prompts/sprint_v1.9.0/H1.2.md`
2. Execute in AutoGen Studio
3. Monitor progress
4. Save log automatically
5. Return summary

---

## ?? **Troubleshooting**

### **"MCP server autogen-studio not found"**

**Fix**: Check path in config.json is correct and absolute.

### **"AutoGen Studio not responding"**

**Fix**: 
```powershell
# Check if AutoGen Studio is running
netstat -ano | findstr :8081

# If not, start it
autogenstudio ui --port 8081 --appdir .
```

### **"Permission denied" errors**

**Fix**: Run as administrator or check file permissions.

---

## ?? **What You Can Do Now**

### **In Copilot Chat**

```
# Execute sprint tasks
"Execute task H1.2 using AutoGen"

# Monitor sessions
"Get status of AutoGen session abc123"

# Run builds
"Run Debug build and report results"

# List teams
"List all AutoGen teams"

# Save logs
"Save AutoGen log for H1.1 with status SUCCESS"
```

---

**Setup time**: ~5 minutes  
**First task execution**: ~3 minutes  
**Total time to productivity**: ~8 minutes

?? **You're ready to use AutoGen via Copilot Chat!**
