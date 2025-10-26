# Est�ndares AAA - Imagine Studio

**OBLIGATORIO**: Este proyecto sigue est�ndares AAA de la industria (Unity, Unreal, Blender). **NUNCA** implementar "apa�os temporales" o "soluciones r�pidas" que comprometan la arquitectura.

---

## ?? REGLA CR�TICA: NO "Apa�os Temporales"

### Filosof�a del Proyecto

> **"Hacer bien las cosas desde el principio"** - Preferencia del propietario

- Cada m�dulo debe cumplir est�ndares AAA desde su **concepci�n**
- **NO** soluciones intermedias, temporales, incompletas o no funcionales
- Si se detecta un "apa�o", **refactorizar a soluci�n AAA inmediatamente**
- Tender SIEMPRE a la arquitectura de Unity/Unreal/Blender (referencias de industria)

---

## ? Ejemplos de "Apa�os" NO Permitidos

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
io.Fonts->Build();  // ? ImGui backends lo hacen autom�ticamente
```

**Problema**: Duplica trabajo, puede causar race conditions con backends.

### 3. **Logs de Debug en Producci�n**

```cpp
// ? NO HACER: Logs en cada frame sin condicional
CORE_LOG_INFO("[NewFrame] io.MouseDown[0]=" << ...);  // ? Sin #ifdef _DEBUG
```

**Problema**: Contamina logs de producci�n, impacto en performance.

### 4. **C�digo Sin Separaci�n de Concerns**

```cpp
// ? NO HACER: L�gica de UI mezclada con l�gica de aplicaci�n
void MainLoop() {
    // C�digo de input, rendering, UI todo mezclado...
}
```

**Problema**: Dificulta mantenimiento, testing, y expansi�n.

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
    
    // Editor UI (NO demo window en producci�n)
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
- ? Separaci�n clara de responsabilidades

### 2. **Separaci�n de Capas (Clean Architecture)**

```
main.cpp (Application Layer)
??? Controla toggle F1
    ??? Pasa estado a Renderer

DX12Renderer (Graphics Layer)
??? Solo renderiza si m_uiVisible = true
    ??? UIPass() ejecuta ImGui_ImplDX12_RenderDrawData()

Editor::EditorUI (Editor Layer)
??? L�gica de panels (Hierarchy, Inspector, Console, Viewport)
    ??? Solo se llama si EDITOR_MODE definido
```

**Beneficios**:
- ? Testabilidad (capas independientes)
- ? Mantenibilidad (responsabilidades claras)
- ? Extensibilidad (a�adir panels sin tocar renderer)

### 3. **Condicionales de Compilaci�n (Debug vs Release)**

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
- ? Logs limpios en producci�n
- ? No contaminar release con c�digo de testing
- ? Performance �ptima en builds finales

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
- ? F�cil de testear y debuggear
- ? Extensible (a�adir m�s controles centralizados)

---

## ?? Checklist Antes de Commit

Antes de crear un commit, verificar:

- [ ] �Esta soluci�n es "temporal" o "definitiva"?
- [ ] �Unity/Unreal implementar�an as� esta funcionalidad?
- [ ] �Hay condicionales de visibilidad donde corresponde?
- [ ] �Los logs de debug est�n bajo `#ifdef _DEBUG`?
- [ ] �La arquitectura tiene separaci�n de concerns clara?
- [ ] �El c�digo es extensible sin modificar arquitectura base?
- [ ] �Se evita c�digo duplicado o "llamar por si acaso"?

**Si la respuesta a cualquier pregunta es NO ? REFACTORIZAR antes de commit**.

---

## ?? Patrones AAA de Referencia

### Unity Editor:
- Panels con docking flexible
- Toggle de UI con tecla (F1/Alt+Enter)
- Separaci�n clara entre Game/Scene/Editor views
- Hot reload sin cerrar aplicaci�n

### Unreal Editor:
- Content Browser separado
- Blueprint vs C++ layers
- Console integrado con logs filtrados
- Viewport como panel m�s (no hardcoded)

### Blender:
- Workspaces predefinidos pero customizables
- Paneles independientes del engine core
- Shortcuts consistentes (F1-F12 asignados l�gicamente)
- UI no bloquea rendering core

---

## ?? Anti-Patrones a Evitar

### 1. **"Quick Fix" Syndrome**
```cpp
// ? A�adir check defensivo sin entender el problema
if (ptr != nullptr) { /* usar ptr */ }  // ? �Por qu� es nullptr?
```
**Correcto**: Investigar POR QU� ptr es nullptr y arreglar causa ra�z.

### 2. **"Works On My Machine"**
```cpp
// ? Hardcodear rutas o configuraciones locales
const char* path = "C:\\Users\\joaqu\\...";
```
**Correcto**: Usar paths relativos o configuraci�n centralizada.

### 3. **"Copy-Paste Engineering"**
```cpp
// ? Copiar c�digo sin entender
// (c�digo de Stack Overflow sin adaptar al proyecto)
```
**Correcto**: Entender, adaptar y documentar c�digo externo.

### 4. **"TODO Later" Hell**
```cpp
// ? Dejar TODOs sin plan de resoluci�n
// TODO: fix this later  ? �Cu�ndo? �C�mo?
```
**Correcto**: Crear issue/task en backlog con plan espec�fico.

---

## ?? Referencias

- `docs/MAIN.md` - Pilares fundamentales del proyecto
- `docs/THIRD_PARTY.md` - Pol�ticas sobre bibliotecas externas
- [Unity Editor Architecture](https://docs.unity3d.com/Manual/UIE-Editor.html)
- [Unreal Engine Editor](https://docs.unrealengine.com/5.3/en-US/editor-fundamentals-in-unreal-engine/)
- [Dear ImGui Best Practices](https://github.com/ocornut/imgui/wiki/Getting-Started)

---

**Versi�n**: v1.0  
**�ltima actualizaci�n**: 2025-01-18  
**Aplica a**: Sprint v1.3.0 en adelante
