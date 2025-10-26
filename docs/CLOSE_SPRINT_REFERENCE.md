###?? IMPORTANTE: Script close-sprint.ps1

**NUNCA** ejecutar comandos PowerShell directamente para archivar sprints.  
**SIEMPRE** usar el script: `.\scripts\close-sprint.ps1 -Version "X.Y.Z"`

**Documentaci�n completa**: Ver `docs/CLOSE_SPRINT.md`

**Uso**: 
```powershell
.\scripts\close-sprint.ps1 -Version "1.3.0"
```

El script archiva autom�ticamente todos los ficheros del sprint a `docs/sprints/` y crea templates vac�os para el siguiente sprint.
