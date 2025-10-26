# Sprint Deviations - v1.3.0

Desviaciones, ajustes arquitect�nicos y tareas emergentes durante el sprint activo v1.3.0 (ImGui Integration).

---

## DEV-001: Refactorizaci�n AAA - Condicional de visibilidad UI y DockSpace

**Tipo**: Ajuste Arquitect�nico (AAA Standards)  
**Detectado en**: H2.3 (despu�s de completar integraci�n DX12 backend)  
**Fecha**: 2025-01-18  
**Prioridad**: CR�TICA  

### Contexto:

Al completar H2.3 (integraci�n del backend DX12 de ImGui), una auditor�a del c�digo detect� que **ImGui se procesaba siempre** (llamando a `NewFrame()` y `Render()` en cada frame) sin respetar el estado `m_uiVisible`, violando est�ndares AAA (Unity/Unreal/Blender style).

**Problemas espec�ficos detectados**:
1. ? `ImGui::NewFrame()` se llamaba incondicionalmente, procesando UI incluso cuando `m_uiVisible = false`
2. ? No hab�a dockspace principal configurado para permitir docking de panels del editor
3. ? `ImGui::ShowDemoWindow()` se mostraba sin condicional de compilaci�n (`#ifdef _DEBUG`)
4. ? Desperdicio de CPU/GPU procesando UI oculta (no respeta F1 toggle)
5. ? Logs de debug sin condicional `#ifdef _DEBUG` contaminando builds release

### �Por qu� NO pas� a backlog?

- ? **Bloqueante para H4** (Editor Panels): Sin arquitectura AAA limpia, los panels no funcionar�an correctamente sobre base deficiente
- ? **Deuda t�cnica inmediata**: El c�digo actual viola pilares fundamentales del proyecto (docs/MAIN.md - "Hacer bien desde el principio")
- ? **Impacto en sprint actual**: H4.1-H4.5 dependen de arquitectura de 3 capas (Application/Renderer/Editor) correctamente implementada
- ? **Alineaci�n con AAA_STANDARDS.md**: Documento reci�n creado define estos est�ndares como obligatorios

### Decisi�n:

**Pausar H4** temporalmente e implementar refactorizaci�n AAA inmediatamente para establecer base s�lida antes de continuar con editor panels.

### Tareas Derivadas:

- **DEV-001.1**: ? A�adir condicional `if (renderer.IsUIVisible())` en main.cpp render loop
  - **Archivos**: `src/main.cpp`
  - **Commit**: 011270b
  
- **DEV-001.2**: ? Implementar `ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport())` para habilitar docking
  - **Archivos**: `src/main.cpp`
  - **Commit**: 011270b
  
- **DEV-001.3**: ? Condicionalizar `ImGui::ShowDemoWindow()` con `#ifdef _DEBUG`
  - **Archivos**: `src/main.cpp`
  - **Commit**: 011270b
  
- **DEV-001.4**: ? Crear documento `docs/AAA_STANDARDS.md` con est�ndares del proyecto
  - **Archivos**: `docs/AAA_STANDARDS.md` (nuevo)
  - **Commit**: 011270b

### Implementaci�n:

**Antes (c�digo problem�tico)**:
```cpp
// ? PROBLEMA: Procesar ImGui siempre, sin condicional
ImGui_ImplWin32_NewFrame();
ImGui_ImplDX12_NewFrame();
ImGui::NewFrame();

// Sin dockspace
ImGui::ShowDemoWindow();  // Sin #ifdef

ImGui::Render();
```

**Despu�s (soluci�n AAA)**:
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

**Validaci�n del usuario**:
- ? Fondo gris del dockspace visible
- ? Docking funcional (puede quitar/modificar opciones de docking)
- ? Render de color con 2 tri�ngulos visible debajo de UI
- ? Capa transl�cida de ImGui sobre el render 3D
- ? F1 toggle funciona correctamente (UI visible/oculta)
- ? Todo funciona como se esperaba ? **OK 100%**

**Compilaci�n**:
- ? CMake Build (Debug): 0 errores, 0 warnings
- ? MSBuild "Imagine Studio.sln" (Debug): 0 errores, 0 warnings

### Impacto en Sprint:

**Tareas afectadas**:
- H4.1 (Panel Hierarchy): Puede continuar sobre arquitectura AAA s�lida
- H4.2 (Panel Inspector): Beneficiado por dockspace habilitado
- H4.3 (Panel Console): Beneficiado por dockspace habilitado
- H4.4 (Panel Viewport): Beneficiado por dockspace habilitado
- H4.5 (Validar UI completa): Base arquitect�nica correcta

**Progreso del sprint**:
- **Antes**: 10/16 tareas (62.5%)
- **Despu�s**: 12/16 tareas (75%) ? +2 tareas (DEV-001.2 dockspace cuenta como H4.5 parcial)

### Lecciones Aprendidas:

1. **Auditor�a post-tarea cr�tica**: Siempre auditar arquitectura despu�s de completar historias complejas (H2). No esperar a H4 para descubrir problemas arquitect�nicos.

2. **Detectar desviaciones AAA temprano**: Implementar checklist de est�ndares AAA antes de marcar historia como "completada".

3. **Documentar decisiones arquitect�nicas**: Crear `sprint_deviations.md` al inicio del sprint para registrar ajustes emergentes formalmente.

4. **"Hacer bien desde el principio"**: La filosof�a del proyecto (docs/MAIN.md) debe aplicarse incluso durante implementaci�n r�pida. Pausar y refactorizar es mejor que acumular deuda t�cnica.

5. **Validaci�n dual (Compilaci�n + Usuario)**: La doble compilaci�n limpia NO garantiza correctitud AAA. La validaci�n del usuario confirm� que el dise�o arquitect�nico es correcto.

---

## Plantilla para Nuevas Desviaciones:

```markdown
## DEV-XXX: [T�tulo de la desviaci�n]

**Tipo**: [Ajuste Arquitect�nico | Tarea Emergente | Deuda T�cnica | Bloqueador]
**Detectado en**: [HX.Y - nombre de tarea]
**Fecha**: [YYYY-MM-DD]
**Prioridad**: [CR�TICA | ALTA | MEDIA | BAJA]

### Contexto:
[Descripci�n del problema detectado]

### �Por qu� NO pas� a backlog?
- Raz�n 1
- Raz�n 2

### Decisi�n:
[Acci�n tomada]

### Tareas Derivadas:
- DEV-XXX.1: [Subtarea 1]
- DEV-XXX.2: [Subtarea 2]

### Implementaci�n:
**Antes**: [C�digo problem�tico]
**Despu�s**: [Soluci�n implementada]

### Resultado:
[Estado actual: En Progreso | Completado | Bloqueado]
[Validaci�n del usuario si aplica]

### Impacto en Sprint:
[Tareas afectadas y progreso actualizado]

### Lecciones Aprendidas:
[Reflexi�n para evitar en futuros sprints]
```

---

## Estad�sticas del Sprint v1.3.0:

**Desviaciones totales**: 1  
**Desviaciones cr�ticas**: 1  
**Desviaciones completadas**: 1  
**Desviaciones pendientes**: 0  

**Impacto en progreso**: +12.5% (de 62.5% a 75%)

---

**Versi�n**: v1.0  
**�ltima actualizaci�n**: 2025-01-18  
**Sprint**: v1.3.0 (ImGui Integration)
