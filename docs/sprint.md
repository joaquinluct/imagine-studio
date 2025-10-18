# Sprint v1.6.0 - Viewport AAA (Offscreen Render Target)

**Versión**: v1.6.0  
**Nombre**: Viewport AAA - Offscreen Render Target  
**Estado**: 🟢 ACTIVO  
**Fecha inicio**: 2025-01-18  
**Fecha estimada fin**: 2025-01-18 (sprint corto, ~3-4 horas)

---

## 🎯 Objetivo del Sprint

Refactorizar el sistema de Viewport para usar un **render target offscreen dedicado** en lugar del swap chain back buffer, eliminando la recursión visual y estableciendo una arquitectura AAA profesional (Unity/Unreal style).

---

## 📋 Historias de Usuario

### **H1: Offscreen Render Target Creation**
**Como** desarrollador del engine  
**Quiero** crear un render target offscreen dedicado para la escena 3D  
**Para** separar el renderizado 3D del renderizado de UI

**Criterios de aceptación**:
- ✅ Render target offscreen creado con flag ALLOW_RENDER_TARGET
- ✅ RTV descriptor creado para renderizar a scene RT
- ✅ SRV descriptor creado para leer scene RT en ImGui::Image()
- ✅ Resolución configurable (inicialmente 1920x1080)
- ✅ Formato DXGI_FORMAT_R8G8B8A8_UNORM

---

### **H2: Refactorizar OpaquePass**
**Como** desarrollador del engine  
**Quiero** que OpaquePass renderice solo a scene RT (no swap chain)  
**Para** que la geometría 3D esté aislada de la UI

**Criterios de aceptación**:
- ✅ OpaquePass usa scene RTV (no back buffer RTV)
- ✅ Clear scene RT con color de fondo (dark blue)
- ✅ Renderizar geometría 3D a scene RT
- ✅ Transición: RENDER_TARGET → PIXEL_SHADER_RESOURCE

---

### **H3: Refactorizar UIPass**
**Como** desarrollador del engine  
**Quiero** que UIPass renderice UI al swap chain con scene RT como textura  
**Para** que el Viewport muestre solo el 3D sin recursión

**Criterios de aceptación**:
- ✅ UIPass usa swap chain RTV (back buffer)
- ✅ Clear swap chain con color del editor (dark gray)
- ✅ Viewport::SetRenderTargetSRV() recibe scene SRV (no swap chain SRV)
- ✅ ImGui::Image() muestra scene RT sin recursión
- ✅ Transición: PRESENT → RENDER_TARGET → PRESENT

---

### **H4: Validación y Testing**
**Como** usuario del editor  
**Quiero** ver el quad 3D en el Viewport sin recursión visual  
**Para** trabajar con una vista limpia de la escena

**Criterios de aceptación**:
- ✅ Viewport muestra SOLO el quad 3D (sin UI recursiva)
- ✅ Otros panels (Hierarchy, Inspector, Console) se ven correctamente
- ✅ Controles de cámara funcionan solo con Viewport hover
- ✅ Resize del Viewport no causa distorsión
- ✅ 60 FPS estables con VSync

---

## 📊 Métricas de Éxito

- **Recursión visual**: Eliminada ✅
- **Separación 3D/UI**: Clara y mantenible ✅
- **Memoria VRAM adicional**: ~8MB (1920x1080 RGBA8)
- **Performance**: Sin impacto (mismo frame time)
- **Compilación**: 0 errores, 0 warnings

---

## 🔗 Dependencias

**Prerequisitos**:
- Sprint v1.5.0 completado ✅
- Conocimiento DX12: CreateCommittedResource, RTV/SRV descriptors

**Bloqueantes**: Ninguno

---

## 📦 Entregables

- Clase `DX12Renderer` refactorizada con scene RT offscreen
- `OpaquePass()` renderiza a scene RT
- `UIPass()` renderiza UI al swap chain con scene texture
- `Viewport` muestra scene RT sin recursión
- Documentación actualizada (commits, daily, sprint_tasks)

---

## ⚠️ Riesgos

| Riesgo | Probabilidad | Impacto | Mitigación |
|--------|--------------|---------|------------|
| Descriptor heap overflow | Baja | Alto | Verificar heap size antes de crear SRV |
| Memory leak scene RT | Media | Alto | Release en Shutdown() |
| Performance regression | Baja | Medio | Profiling antes/después |

---

## 🎯 Definición de Hecho (DoD)

- ✅ Código compila limpio (0 errores, 0 warnings)
- ✅ Viewport sin recursión visual (validado por usuario)
- ✅ Controles de cámara funcionan correctamente
- ✅ Documentación actualizada (commits, sprint_tasks, daily)
- ✅ Commit final de cierre de sprint
- ✅ BACK-008 marcado como resuelto en backlog

---

## 📈 Progreso

**Estado actual**: 0/8 tareas completadas (0%)

```
┌────────────────────────────────────────────────────────────────────┐
│⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛ 0.0%│
└────────────────────────────────────────────────────────────────────┘
```

---

## 📝 Notas

- Sprint corto enfocado (3-4 horas estimadas)
- Alto impacto visual (elimina recursión)
- Base para features futuros (post-processing, multi-viewport, deferred rendering)
- Resuelve BACK-008 del backlog

---

**Versión**: 1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.6.0 (Viewport AAA) - **EN PROGRESO**
