# Sprint v1.6.0 - Viewport AAA (Offscreen Render Target)

**Versión**: v1.6.0  
**Nombre**: Viewport AAA - Offscreen Render Target  
**Estado**: ✅ **CERRADO**  
**Fecha inicio**: 2025-01-18  
**Fecha fin**: 2025-01-18  
**Duración real**: ~4 horas (estimado: 3-4 horas) ✅

---

## 🎯 Objetivo del Sprint

Refactorizar el sistema de Viewport para usar un **render target offscreen dedicado** en lugar del swap chain back buffer, eliminando la recursión visual y estableciendo una arquitectura AAA profesional (Unity/Unreal style).

**Resultado**: ✅ **COMPLETADO AL 100%**

---

## 📋 Historias de Usuario

### **H1: Offscreen Render Target Creation** ✅ COMPLETADA
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

### **H2: Refactorizar OpaquePass** ✅ COMPLETADA
**Como** desarrollador del engine  
**Quiero** que OpaquePass renderice solo a scene RT (no swap chain)  
**Para** que la geometría 3D esté aislada de la UI

**Criterios de aceptación**:
- ✅ OpaquePass usa scene RTV (no back buffer RTV)
- ✅ Clear scene RT con color de fondo (dark blue)
- ✅ Renderizar geometría 3D a scene RT
- ✅ Transición: RENDER_TARGET → PIXEL_SHADER_RESOURCE

---

### **H3: Refactorizar UIPass** ✅ COMPLETADA
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

### **H4: Validación y Testing** ✅ COMPLETADA
**Como** usuario del editor  
**Quiero** ver el quad 3D en el Viewport sin recursión visual  
**Para** trabajar con una vista limpia de la escena

**Criterios de aceptación**:
- ✅ Viewport muestra SOLO el quad 3D (sin UI recursiva)
- ✅ Otros panels (Hierarchy, Inspector, Console) se ven correctamente
- ✅ Controles de cámara funcionan solo con Viewport hover
- ✅ Quad visible con interpolación de colores correcta (rojo, azul, verde, amarillo)
- ✅ 220 FPS estables (sin VSync)

---

## 📊 Métricas de Éxito

- **Recursión visual**: ✅ Eliminada
- **Separación 3D/UI**: ✅ Clara y mantenible
- **Memoria VRAM adicional**: ~8MB (1920x1080 RGBA8) - Aceptable
- **Performance**: ✅ Sin impacto (220 FPS)
- **Compilación**: ✅ 0 errores, 0 warnings

---

## 🐛 **Bugs Encontrados y Resueltos**

### **BUG-4: Quad no visible en Viewport** ✅ RESUELTO
**Causa raíz**:
1. Resource state transitions incompletas (back buffer + scene RT)
2. Upload buffer liberado antes de que GPU terminara copia

**Solución**:
1. Resource barriers completos con flag `isFirstFrame`
2. Upload buffer mantenido vivo hasta `WaitForGPU()`

**Intentos**: 5 (documentados en `docs/sprint_bug_attempts.md`)  
**Commits**: `85f190d`, `46fb951`, `2723e30`

Ver detalles completos en: `docs/sprint_fix.md`

---

## 🔗 Dependencias

**Prerequisitos**:
- Sprint v1.5.0 completado ✅

**Bloqueantes**: Ninguno

---

## 📦 Entregables

- ✅ Clase `DX12Renderer` refactorizada con scene RT offscreen
- ✅ `DX12ResourceManager` con upload buffer lifetime management
- ✅ `OpaquePass` renderiza a scene RT
- ✅ `UIPass` renderiza UI al swap chain con scene texture
- ✅ `Viewport` muestra scene RT sin recursión
- ✅ Resource barriers completos (back buffer + scene RT)
- ✅ Documentación actualizada (commits, daily, sprint docs)

---

## ⚠️ Riesgos

| Riesgo | Probabilidad | Impacto | Estado |
|--------|--------------|---------|--------|
| Descriptor heap overflow | Baja | Alto | ✅ Mitigado |
| Memory leak scene RT | Media | Alto | ✅ Resuelto (Release en Shutdown) |
| Upload buffer premature release | Alta | Crítico | ✅ Resuelto (BUG-4) |
| Performance regression | Baja | Medio | ✅ No ocurrió |

---

## 🎯 Definición de Hecho (DoD)

- ✅ Código compila limpio (0 errores, 0 warnings)
- ✅ Viewport sin recursión visual (validado por usuario)
- ✅ Quad visible con colores correctos
- ✅ Controles de cámara funcionan correctamente
- ✅ Documentación actualizada (commits, sprint_tasks, daily)
- ✅ Commits pusheados a GitHub (`85f190d`, `46fb951`, `2723e30`)
- ✅ BUG-4 marcado como resuelto en `sprint_fix.md`

---

## 📈 Progreso Final

**Estado final**: 20/20 tareas completadas (100%) ✅

```
┌────────────────────────────────────────────────────────────────────┐
│🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩 100%│
└────────────────────────────────────────────────────────────────────┘
```

---

## 🏆 **Logros del Sprint**

1. ✅ **Arquitectura AAA establecida**: Offscreen render target + dual-pass rendering
2. ✅ **Bug crítico resuelto**: Upload buffer lifetime management (patrón AAA)
3. ✅ **Resource barriers correctos**: Back buffer + scene RT state transitions
4. ✅ **Viewport funcional**: Quad visible sin recursión, interpolación de colores
5. ✅ **Performance estable**: 220 FPS (sin VSync)

---

## 📝 **Lecciones Aprendidas**

### **1. DirectX 12 - Upload Buffer Lifetime**
Upload buffers DEBEN mantenerse vivos hasta que GPU termine de leer de ellos.

**Secuencia correcta**:
1. Crear upload buffer
2. Grabar `CopyBufferRegion()`
3. `Execute()` command list
4. `WaitForGPU()`
5. **SOLO ENTONCES** liberar upload buffer

### **2. Resource State Transitions - Primer Frame**
Scene RT se crea en estado `RENDER_TARGET`, no necesita transición en frame 1.  
Usar flag `isFirstFrame` para evitar barrier mismatch.

### **3. Debugging Sistemático**
5 intentos documentados permitieron identificar AMBOS problemas (barriers + upload buffer) de forma metódica.

---

## 🚀 **Siguiente Sprint**

**Sprint v1.7.0 - Performance Optimization** planificado.

**Objetivos**:
1. Frame pipelining (+300% FPS)
2. Command allocator ring buffer (+20% FPS)
3. Barrier batching (+15% FPS)

**Performance esperada**: De 220 FPS → 800-1200 FPS (nivel AAA)

---

**Versión**: 1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.6.0 (Viewport AAA) - ✅ **CERRADO**
