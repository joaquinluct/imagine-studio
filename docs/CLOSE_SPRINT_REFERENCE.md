###?? IMPORTANTE: Script close-sprint.ps1

**NUNCA** ejecutar comandos PowerShell directamente para archivar sprints.  
**SIEMPRE** usar el script: `.\scripts\close-sprint.ps1 -Version "X.Y.Z"`

**Documentación completa**: Ver `docs/CLOSE_SPRINT.md`

**Uso**: 
```powershell
.\scripts\close-sprint.ps1 -Version "1.3.0"
```

El script archiva automáticamente todos los ficheros del sprint a `docs/sprints/` y crea templates vacíos para el siguiente sprint.
