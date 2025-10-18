# Estándares AAA - Imagine Studio

**OBLIGATORIO**: Este proyecto sigue estándares AAA de la industria (Unity, Unreal, Blender). **NUNCA** implementar "apaños temporales" o "soluciones rápidas" que comprometan la arquitectura.

---

## ?? REGLA CRÍTICA: NO "Apaños Temporales"

### Filosofía del Proyecto

> **"Hacer bien las cosas desde el principio"** - Preferencia del propietario

- Cada módulo debe cumplir estándares AAA desde su **concepción**
- **NO** soluciones intermedias, temporales, incompletas o no funcionales
- Si se detecta un "apaño", **refactorizar a solución AAA inmediatamente**
- Tender SIEMPRE a la arquitectura de Unity/Unreal/Blender (referencias de industria)

---

## ? Ejemplos de "Apaños" NO Permitidos

### 1. **Procesar UI Siempre (Sin Condicional de Visibilidad)**

```cpp
// ? NO HACER: Procesar ImGui aunque UI no visible
ImGui_ImplWin32_NewFrame();
ImGui_ImplDX12_NewFrame();
ImGui::NewFrame();
// ... UI logic ...
ImGui::Render();  // ? Se ejecuta SIEMPRE, desperdicio de CPU/GPU
```

**Problema**: Desperdicia recursos procesando UI invisible, no respeta F1 toggle.

### 2. **Llamar Funciones "Por Si Acaso"**

```cpp
// ? NO HACER: Llamar Build() manualmente "por si acaso"
io.Fonts->Build();  // ? ImGui backends lo hacen automáticamente
```

**Problema**: Duplica trabajo, puede causar race conditions con backends.

### 3. **Logs de Debug en Producción**

```cpp
// ? NO HACER: Logs en cada frame sin condicional
CORE_LOG_INFO("[NewFrame] io.MouseDown[0]=" << ...);  // ? Sin #ifdef _DEBUG
```

**Problema**: Contamina logs de producción, impacto en performance.

### 4. **Código Sin Separación de Concerns**

```cpp
// ? NO HACER: Lógica de UI mezclada con lógica de aplicación
void MainLoop() {
    // Código de input, rendering, UI todo mezclado...
}
```

**Problema**: Dificulta mantenimiento, testing, y expansión.

---

## ? Soluciones AAA Correctas

### 1. **Condicional de Visibilidad (Unity/Unreal Style)**

```cpp
// ? CORRECTO: Solo procesar si UI visible
if (renderer.IsUIVisible())  // ? F1 toggle controla esto
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX12_NewFrame();
    ImGui::NewFrame();
    
    // Dockspace ANTES de panels (AAA standard)
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    
    // Editor UI (NO demo window en producción)
    #ifdef EDITOR_MODE
        Editor::EditorUI::RenderAllPanels();
    #endif
    
    ImGui::Render();
}
// Si UI no visible, NO procesamos ImGui
```

**Beneficios**:
- ? Ahorro de CPU/GPU cuando UI oculta
- ? Respeta toggle F1 correctamente
- ? Separación clara de responsabilidades

### 2. **Separación de Capas (Clean Architecture)**

```
main.cpp (Application Layer)
??? Controla toggle F1
    ??? Pasa estado a Renderer

DX12Renderer (Graphics Layer)
??? Solo renderiza si m_uiVisible = true
    ??? UIPass() ejecuta ImGui_ImplDX12_RenderDrawData()

Editor::EditorUI (Editor Layer)
??? Lógica de panels (Hierarchy, Inspector, Console, Viewport)
    ??? Solo se llama si EDITOR_MODE definido
```

**Beneficios**:
- ? Testabilidad (capas independientes)
- ? Mantenibilidad (responsabilidades claras)
- ? Extensibilidad (añadir panels sin tocar renderer)

### 3. **Condicionales de Compilación (Debug vs Release)**

```cpp
// ? CORRECTO: Debug logs solo en builds debug
#ifdef _DEBUG
    CORE_LOG_DEBUG("[NewFrame] io.MouseDown[0]=" << io.MouseDown[0]);
#endif

// ? CORRECTO: Demo window solo en desarrollo
#ifdef _DEBUG
    ImGui::ShowDemoWindow();
#else
    #ifdef EDITOR_MODE
        Editor::EditorUI::RenderAllPanels();  // Production Editor
    #endif
#endif
```

**Beneficios**:
- ? Logs limpios en producción
- ? No contaminar release con código de testing
- ? Performance óptima en builds finales

### 4. **Control de Estado Centralizado**

```cpp
// ? CORRECTO: Estado controlado en renderer
class DX12Renderer {
private:
    bool m_uiVisible = true;  // F1 toggle controla esto
    
public:
    void ToggleUI() { m_uiVisible = !m_uiVisible; }
    bool IsUIVisible() const { return m_uiVisible; }
};

// Application layer usa el getter
if (renderer.IsUIVisible()) {
    // Procesar UI
}
```

**Beneficios**:
- ? Single source of truth
- ? Fácil de testear y debuggear
- ? Extensible (añadir más controles centralizados)

---

## ?? Checklist Antes de Commit

Antes de crear un commit, verificar:

- [ ] ¿Esta solución es "temporal" o "definitiva"?
- [ ] ¿Unity/Unreal implementarían así esta funcionalidad?
- [ ] ¿Hay condicionales de visibilidad donde corresponde?
- [ ] ¿Los logs de debug están bajo `#ifdef _DEBUG`?
- [ ] ¿La arquitectura tiene separación de concerns clara?
- [ ] ¿El código es extensible sin modificar arquitectura base?
- [ ] ¿Se evita código duplicado o "llamar por si acaso"?

**Si la respuesta a cualquier pregunta es NO ? REFACTORIZAR antes de commit**.

---

## ?? Patrones AAA de Referencia

### Unity Editor:
- Panels con docking flexible
- Toggle de UI con tecla (F1/Alt+Enter)
- Separación clara entre Game/Scene/Editor views
- Hot reload sin cerrar aplicación

### Unreal Editor:
- Content Browser separado
- Blueprint vs C++ layers
- Console integrado con logs filtrados
- Viewport como panel más (no hardcoded)

### Blender:
- Workspaces predefinidos pero customizables
- Paneles independientes del engine core
- Shortcuts consistentes (F1-F12 asignados lógicamente)
- UI no bloquea rendering core

---

## ?? Anti-Patrones a Evitar

### 1. **"Quick Fix" Syndrome**
```cpp
// ? Añadir check defensivo sin entender el problema
if (ptr != nullptr) { /* usar ptr */ }  // ? ¿Por qué es nullptr?
```
**Correcto**: Investigar POR QUÉ ptr es nullptr y arreglar causa raíz.

### 2. **"Works On My Machine"**
```cpp
// ? Hardcodear rutas o configuraciones locales
const char* path = "C:\\Users\\joaqu\\...";
```
**Correcto**: Usar paths relativos o configuración centralizada.

### 3. **"Copy-Paste Engineering"**
```cpp
// ? Copiar código sin entender
// (código de Stack Overflow sin adaptar al proyecto)
```
**Correcto**: Entender, adaptar y documentar código externo.

### 4. **"TODO Later" Hell**
```cpp
// ? Dejar TODOs sin plan de resolución
// TODO: fix this later  ? ¿Cuándo? ¿Cómo?
```
**Correcto**: Crear issue/task en backlog con plan específico.

---

## ?? Referencias

- `docs/MAIN.md` - Pilares fundamentales del proyecto
- `docs/THIRD_PARTY.md` - Políticas sobre bibliotecas externas
- [Unity Editor Architecture](https://docs.unity3d.com/Manual/UIE-Editor.html)
- [Unreal Engine Editor](https://docs.unrealengine.com/5.3/en-US/editor-fundamentals-in-unreal-engine/)
- [Dear ImGui Best Practices](https://github.com/ocornut/imgui/wiki/Getting-Started)

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Aplica a**: Sprint v1.3.0 en adelante
