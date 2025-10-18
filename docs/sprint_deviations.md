# Sprint Deviations - v1.3.0

Desviaciones, ajustes arquitectónicos y tareas emergentes durante el sprint activo v1.3.0 (ImGui Integration).

---

## DEV-001: Refactorización AAA - Condicional de visibilidad UI y DockSpace

**Tipo**: Ajuste Arquitectónico (AAA Standards)  
**Detectado en**: H2.3 (después de completar integración DX12 backend)  
**Fecha**: 2025-01-18  
**Prioridad**: CRÍTICA  

### Contexto:

Al completar H2.3 (integración del backend DX12 de ImGui), una auditoría del código detectó que **ImGui se procesaba siempre** (llamando a `NewFrame()` y `Render()` en cada frame) sin respetar el estado `m_uiVisible`, violando estándares AAA (Unity/Unreal/Blender style).

**Problemas específicos detectados**:
1. ? `ImGui::NewFrame()` se llamaba incondicionalmente, procesando UI incluso cuando `m_uiVisible = false`
2. ? No había dockspace principal configurado para permitir docking de panels del editor
3. ? `ImGui::ShowDemoWindow()` se mostraba sin condicional de compilación (`#ifdef _DEBUG`)
4. ? Desperdicio de CPU/GPU procesando UI oculta (no respeta F1 toggle)
5. ? Logs de debug sin condicional `#ifdef _DEBUG` contaminando builds release

### ¿Por qué NO pasó a backlog?

- ? **Bloqueante para H4** (Editor Panels): Sin arquitectura AAA limpia, los panels no funcionarían correctamente sobre base deficiente
- ? **Deuda técnica inmediata**: El código actual viola pilares fundamentales del proyecto (docs/MAIN.md - "Hacer bien desde el principio")
- ? **Impacto en sprint actual**: H4.1-H4.5 dependen de arquitectura de 3 capas (Application/Renderer/Editor) correctamente implementada
- ? **Alineación con AAA_STANDARDS.md**: Documento recién creado define estos estándares como obligatorios

### Decisión:

**Pausar H4** temporalmente e implementar refactorización AAA inmediatamente para establecer base sólida antes de continuar con editor panels.

### Tareas Derivadas:

- **DEV-001.1**: ? Añadir condicional `if (renderer.IsUIVisible())` en main.cpp render loop
  - **Archivos**: `src/main.cpp`
  - **Commit**: 011270b
  
- **DEV-001.2**: ? Implementar `ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport())` para habilitar docking
  - **Archivos**: `src/main.cpp`
  - **Commit**: 011270b
  
- **DEV-001.3**: ? Condicionalizar `ImGui::ShowDemoWindow()` con `#ifdef _DEBUG`
  - **Archivos**: `src/main.cpp`
  - **Commit**: 011270b
  
- **DEV-001.4**: ? Crear documento `docs/AAA_STANDARDS.md` con estándares del proyecto
  - **Archivos**: `docs/AAA_STANDARDS.md` (nuevo)
  - **Commit**: 011270b

### Implementación:

**Antes (código problemático)**:
```cpp
// ? PROBLEMA: Procesar ImGui siempre, sin condicional
ImGui_ImplWin32_NewFrame();
ImGui_ImplDX12_NewFrame();
ImGui::NewFrame();

// Sin dockspace
ImGui::ShowDemoWindow();  // Sin #ifdef

ImGui::Render();
```

**Después (solución AAA)**:
```cpp
// ? CORRECTO: Solo procesar si UI visible
if (renderer.IsUIVisible())
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX12_NewFrame();
    ImGui::NewFrame();
    
    // Dockspace principal (Unity/Unreal style)
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    
    #ifdef _DEBUG
        ImGui::ShowDemoWindow();  // Solo en debug
    #else
        // TODO H4: Editor::EditorUI::RenderAllPanels()
    #endif
    
    ImGui::Render();
}
```

### Resultado:

? **Completado** (commit: 011270b)

**Validación del usuario**:
- ? Fondo gris del dockspace visible
- ? Docking funcional (puede quitar/modificar opciones de docking)
- ? Render de color con 2 triángulos visible debajo de UI
- ? Capa translúcida de ImGui sobre el render 3D
- ? F1 toggle funciona correctamente (UI visible/oculta)
- ? Todo funciona como se esperaba ? **OK 100%**

**Compilación**:
- ? CMake Build (Debug): 0 errores, 0 warnings
- ? MSBuild "Imagine Studio.sln" (Debug): 0 errores, 0 warnings

### Impacto en Sprint:

**Tareas afectadas**:
- H4.1 (Panel Hierarchy): Puede continuar sobre arquitectura AAA sólida
- H4.2 (Panel Inspector): Beneficiado por dockspace habilitado
- H4.3 (Panel Console): Beneficiado por dockspace habilitado
- H4.4 (Panel Viewport): Beneficiado por dockspace habilitado
- H4.5 (Validar UI completa): Base arquitectónica correcta

**Progreso del sprint**:
- **Antes**: 10/16 tareas (62.5%)
- **Después**: 12/16 tareas (75%) ? +2 tareas (DEV-001.2 dockspace cuenta como H4.5 parcial)

### Lecciones Aprendidas:

1. **Auditoría post-tarea crítica**: Siempre auditar arquitectura después de completar historias complejas (H2). No esperar a H4 para descubrir problemas arquitectónicos.

2. **Detectar desviaciones AAA temprano**: Implementar checklist de estándares AAA antes de marcar historia como "completada".

3. **Documentar decisiones arquitectónicas**: Crear `sprint_deviations.md` al inicio del sprint para registrar ajustes emergentes formalmente.

4. **"Hacer bien desde el principio"**: La filosofía del proyecto (docs/MAIN.md) debe aplicarse incluso durante implementación rápida. Pausar y refactorizar es mejor que acumular deuda técnica.

5. **Validación dual (Compilación + Usuario)**: La doble compilación limpia NO garantiza correctitud AAA. La validación del usuario confirmó que el diseño arquitectónico es correcto.

---

## Plantilla para Nuevas Desviaciones:

```markdown
## DEV-XXX: [Título de la desviación]

**Tipo**: [Ajuste Arquitectónico | Tarea Emergente | Deuda Técnica | Bloqueador]
**Detectado en**: [HX.Y - nombre de tarea]
**Fecha**: [YYYY-MM-DD]
**Prioridad**: [CRÍTICA | ALTA | MEDIA | BAJA]

### Contexto:
[Descripción del problema detectado]

### ¿Por qué NO pasó a backlog?
- Razón 1
- Razón 2

### Decisión:
[Acción tomada]

### Tareas Derivadas:
- DEV-XXX.1: [Subtarea 1]
- DEV-XXX.2: [Subtarea 2]

### Implementación:
**Antes**: [Código problemático]
**Después**: [Solución implementada]

### Resultado:
[Estado actual: En Progreso | Completado | Bloqueado]
[Validación del usuario si aplica]

### Impacto en Sprint:
[Tareas afectadas y progreso actualizado]

### Lecciones Aprendidas:
[Reflexión para evitar en futuros sprints]
```

---

## Estadísticas del Sprint v1.3.0:

**Desviaciones totales**: 1  
**Desviaciones críticas**: 1  
**Desviaciones completadas**: 1  
**Desviaciones pendientes**: 0  

**Impacto en progreso**: +12.5% (de 62.5% a 75%)

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.3.0 (ImGui Integration)
