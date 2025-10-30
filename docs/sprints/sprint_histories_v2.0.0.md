# Sprint Histories - v2.0.0 (Active)

> **Referencia**: Ver [`docs/sprint_tasks.md`](sprint_tasks.md) para tareas detalladas del sprint.

Este archivo contiene las historias de usuario (alto nivel) del sprint activo v2.0.0 - Material System (PBR).

---

## Historia 1: Material Core (H1)

### H1 - Material Core
**ID**: H1  
**Prioridad**: ?? Crítica  
**Estado**: ? Pendiente  

**Descripción**: Implementar clases base del sistema de materiales (Material, MaterialInstance, MaterialManager) con propiedades PBR estándar.

**Criterios de aceptación**:
- [ ] Material class con MaterialProperties struct (albedo, metallic, roughness)
- [ ] 5 texture slots: Albedo, Normal, Roughness, Metallic, AO
- [ ] Valores default: albedo (1,1,1,1), metallic (0.0), roughness (0.5)
- [ ] MaterialInstance para crear variantes sin duplicar shaders
- [ ] MaterialManager singleton con cache de materiales
- [ ] Thread-safe access a materiales

**Tareas derivadas**: H1.1, H1.2, H1.3, H1.4

---

## Historia 2: PBR Shader Pipeline (H2)

### H2 - PBR Shader Pipeline
**ID**: H2  
**Prioridad**: ?? Crítica  
**Estado**: ? Pendiente  

**Descripción**: Implementar shaders PBR en DirectX 12 con Cook-Torrance BRDF y soporte de texturas.

**Criterios de aceptación**:
- [ ] PBR vertex shader con TBN matrix para normal mapping
- [ ] PBR pixel shader con Cook-Torrance BRDF
- [ ] Constant buffers: Material CB (properties), Light CB (directional light)
- [ ] Descriptor heap con SRVs para 5 texturas por material
- [ ] PSO (Pipeline State Object) configurado correctamente
- [ ] Fallback a valores default si textura no presente
- [ ] Performance: <1ms render time para 1000 objetos

**Tareas derivadas**: H2.1, H2.2, H2.3, H2.4, H2.5

---

## Historia 3: Material Editor Panel (H3)

### H3 - Material Editor Panel
**ID**: H3  
**Prioridad**: ?? Alta  
**Estado**: ? Pendiente  

**Descripción**: Implementar panel ImGui para editar materiales en tiempo real con drag & drop de texturas.

**Criterios de aceptación**:
- [ ] MaterialEditor panel visible en editor
- [ ] 5 texture slots con thumbnails 64x64
- [ ] Drag & drop texturas desde Asset Browser funcional
- [ ] Sliders 0.0-1.0 para metallic/roughness
- [ ] Color picker para albedo tint
- [ ] Preview thumbnail del material actualizado en real-time
- [ ] Botón "Save Material" guarda .mat file

**Tareas derivadas**: H3.1, H3.2, H3.3, H3.4

---

## Historia 4: Material Assignment (H4)

### H4 - Material Assignment
**ID**: H4  
**Prioridad**: ?? Alta  
**Estado**: ? Pendiente  

**Descripción**: Conectar sistema de materiales con Entity System para aplicar materiales a meshes.

**Criterios de aceptación**:
- [ ] MeshRenderer component almacena Material* pointer
- [ ] Inspector muestra material asignado (nombre + preview thumbnail)
- [ ] Drag & drop material desde Asset Browser a Inspector
- [ ] Rendering pipeline aplica texturas del material (bind SRVs)
- [ ] Constant buffer actualizado con MaterialProperties
- [ ] Cambiar material actualiza rendering inmediatamente

**Tareas derivadas**: H4.1, H4.2, H4.3

---

## Historia 5: Material Serialization & Hot-Reload (H5)

### H5 - Material Serialization & Hot-Reload
**ID**: H5  
**Prioridad**: ?? Media  
**Estado**: ? Pendiente  

**Descripción**: Guardar materiales en formato JSON y hot-reload automático al cambiar texturas en disco.

**Criterios de aceptación**:
- [ ] SaveMaterial() guarda .mat file en assets/materials/
- [ ] LoadMaterial() carga .mat desde JSON
- [ ] Formato JSON con texture paths y properties
- [ ] FileWatcher detecta cambios en texturas (Win32 API)
- [ ] Auto-reload material en <100ms
- [ ] Log en Console: "Material reloaded: brick.mat"
- [ ] Hot-reload sin restart de aplicación

**Tareas derivadas**: H5.1, H5.2, H5.3

---

## ?? Resumen de Historias

| ID | Historia | Tareas | Completadas | Estado |
|----|----------|--------|-------------|--------|
| H1 | Material Core | 4 | 0 | ? Pendiente |
| H2 | PBR Shader Pipeline | 5 | 0 | ? Pendiente |
| H3 | Material Editor Panel | 4 | 0 | ? Pendiente |
| H4 | Material Assignment | 3 | 0 | ? Pendiente |
| H5 | Material Serialization & Hot-Reload | 3 | 0 | ? Pendiente |

**Total historias**: 5 (0 completadas, 5 pendientes)  
**Total tareas**: 19 (0 completadas, 19 pendientes)

---

*Última actualización*: 2025-01-21  
*Sprint*: v2.0.0 - Material System (PBR)
