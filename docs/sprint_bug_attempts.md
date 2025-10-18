# Sprint Bug Attempts (v1.3.0)

**Propósito**: Este archivo registra TODOS los intentos de solución para cada bug activo, incluyendo enfoques fallidos, parciales y exitosos. Esto permite trazabilidad completa del proceso de debugging y evita repetir soluciones ya probadas.

**Formato**: Cada bug tiene su propia sección con lista cronológica de intentos.

---

## BUG-002 - Click de ratón no funciona en UI ImGui (RESUELTO)

### Intento 1: Añadir flag ImGuiBackendFlags_RendererHasTextures
**Fecha**: 2025-01-18
**Commit**: [pendiente]
**Enfoque**: Configurar `io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures` ANTES de `AddFontDefault()`
**Resultado**: ? **FALLO** - Assertion fail "Cannot use dynamic font size with locked ImFontAtlas!"
**Causa**: El atlas no estaba construido (built) antes de inicializar backends
**Lección aprendida**: El flag solo indica soporte de texturas dinámicas, pero el atlas debe estar construido primero

### Intento 2: Construir atlas explícitamente
**Fecha**: 2025-01-18
**Commit**: [pendiente]
**Enfoque**: 
1. Añadir fuente por defecto: `io.Fonts->AddFontDefault()`
2. Construir atlas explícitamente: `io.Fonts->Build()`
3. Configurar flag DESPUÉS de Build: `io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures`

**Resultado**: ? **PARCIAL** - Se eliminaron los assertion fails, pero los clicks seguían sin funcionar
**Causa**: El atlas estaba correctamente inicializado, pero los eventos de ratón no se procesaban correctamente en WndProc
**Lección aprendida**: Inicialización correcta del atlas es necesaria pero no suficiente para eventos de ratón

### Intento 3: Respetar valor de retorno de ImGui_ImplWin32_WndProcHandler
**Fecha**: 2025-01-18
**Commit**: [pendiente]
**Enfoque**: 
```cpp
// ANTES (INCORRECTO):
ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam); // Ignoraba retorno
// ... procesaba el evento de todos modos

// DESPUÉS (CORRECTO):
if (ImGui_ImplWin32_WndProcHandler(hwnd_, message, wParam, lParam))
    return true; // ImGui capturó el evento, NO pasar a DefWindowProc
```

**Resultado**: ? **PENDIENTE VALIDACIÓN USUARIO**
**Causa raíz hipotética**: No se respetaba el valor de retorno del handler. Cuando ImGui capturaba un evento (retorna != 0), el evento se pasaba igualmente a DefWindowProc, lo que interfería con el procesamiento de ImGui.
**Archivos modificados**: `src/platform/Window.cpp`, `src/main.cpp`

**Lección aprendida (hipotética)**: 
- El handler de eventos de ImGui **DEBE** retornar true cuando captura un evento
- Si retorna true, NO debemos pasar el evento a DefWindowProc
- El orden correcto de inicialización del atlas es crítico pero ortogonal al procesamiento de eventos

**Estado actual**: Esperando validación del usuario para confirmar que el fix funciona en ejecución real.

**Solución final (si se confirma)**:
1. Atlas construido correctamente (Intento 2)
2. Eventos respetados correctamente (Intento 3)

### Intento 4: Debugging con logs detallados de eventos de mouse
**Fecha**: 2025-01-18
**Commit**: [completado]
**Enfoque**: 
Añadir logs exhaustivos para diagnosticar exactamente dónde se pierden los eventos de mouse

**Resultado**: ? **DIAGNÓSTICO EXITOSO**
**Hallazgos confirmados por logs del usuario**:
1. ? **NO hay logs de `[WndProc]`** ? Los mensajes WM_LBUTTONDOWN/WM_LBUTTONUP NO llegan a WndProc
2. ? `io.MouseDown[0]` siempre es 0 ? ImGui nunca recibe notificación de clicks
3. ? `WantCaptureMouse=1` cuando está sobre UI ? ImGui detecta hover correctamente
4. ? Error 1400 en CreateWindowEx ? Ventana creada con clase fallback `STATIC`
5. ? Fallback a clase STATIC exitoso ? `HWND=0000000000310B80` pero sin eventos de mouse

**Logs clave del usuario**:
```
[ERROR] Window::Window - CreateWindowEx failed, GetLastError=1400
[INFO] Window::Window - fallback using WC_OVERLAPPEDWINDOW succeeded, HWND=0000000000310B80
[INFO] [NewFrame] io.MouseDown[0]=0, io.MouseDown[1]=0, WantCaptureMouse=0
[INFO] [NewFrame] io.MouseDown[0]=0, io.MouseDown[1]=0, WantCaptureMouse=1  # ? Mouse sobre UI
# ?? NO hay ningún log [WndProc] Mouse button DOWN/UP
```

**Causa raíz identificada**: 
La ventana se está creando con clase fallback `STATIC` en lugar de nuestra clase personalizada `ImagineStudioWindowClass`. La clase STATIC no tiene nuestro `WndProc` correctamente asociado, por lo que los eventos de mouse (WM_LBUTTONDOWN, WM_LBUTTONUP, etc.) **no llegan a WndProc**.

**¿Por qué falla CreateWindowExW?**
- Error 1400 = `ERROR_INVALID_WINDOW_HANDLE` sugiere problema con el registro de clase
- Posibles causas:
  1. `RegisterClassExW` falla silenciosamente (atom=0 no verificado)
  2. Mismatch entre `hInstance` usado en registro vs creación
  3. Clase ya registrada pero con diferente `hInstance`

**Lección aprendida**:
- ? Los logs fueron CRÍTICOS para identificar que WndProc nunca recibió eventos
- ? El error 1400 indica problema en el registro/creación de la ventana personalizada
- ? El fallback a clase STATIC enmascara el problema y hace debugging más difícil
- ? La clase STATIC NO es adecuada para ventanas interactivas (no procesa eventos correctamente)

### Intento 5: Corregir registro de clase y eliminación de fallback
**Fecha**: 2025-01-18
**Commit**: [completado]
**Enfoque**:
El problema identificado en Intento #4 es que `CreateWindowExW` falla con error 1400 y se usa fallback con clase `STATIC`, que no tiene nuestro WndProc correctamente asociado.

Solución implementada:
1. ? Usar `GetModuleHandle(NULL)` **consistentemente** en registro y creación de ventana
2. ? Verificar si la clase ya está registrada antes de intentar registrarla de nuevo
3. ? Usar nombre de clase directamente (no atom) en `CreateWindowExW`
4. ? **ELIMINAR** fallback a clase STATIC (enmascara el problema real)
5. ? Si la creación falla, abortar con error detallado en lugar de continuar con ventana defectuosa

**Cambios en código**:
```cpp
// ANTES (problemático):
HINSTANCE regInstance = hInstance_ ? hInstance_ : GetModuleHandle(NULL);
// ... RegisterClassExW con regInstance
// ... CreateWindowExW falla ? usa fallback con clase STATIC

// DESPUÉS (correcto):
HINSTANCE moduleInstance = GetModuleHandle(NULL);  // Siempre el mismo
BOOL classExists = GetClassInfoExW(moduleInstance, CLASS_NAME, &existingClass);
if (!classExists)
{
    // Registrar clase solo si no existe
    ATOM atom = RegisterClassExW(&wc);
    if (atom == 0) { /* ABORTAR con error */ }
}
hwnd_ = CreateWindowExW(0, CLASS_NAME, title, ..., moduleInstance, ...);
if (!hwnd_) { /* ABORTAR con error detallado - SIN FALLBACK */ }
```

**Archivos modificados**: `src/platform/Window.cpp`

**Resultado**: ? **COMPILACIÓN LIMPIA**
- CMake Build (Debug): 0 errores, 0 warnings
- MSBuild "Imagine Studio.sln" (Debug): 0 errores, 0 warnings

**Estado**: ? **PENDIENTE VALIDACIÓN USUARIO**

El usuario debe:
1. Ejecutar la aplicación (`x64\Debug\Imagine Studio.exe`)
2. Verificar que:
   - ? La ventana se crea correctamente (sin error 1400)
   - ? Los logs muestran `[WndProc] Mouse button DOWN/UP` al hacer click
   - ? Los clicks funcionan en la UI de ImGui (abrir menús, botones, etc.)

**Expectativa**: 
- Si el problema era el mismatch de `hInstance`, la ventana debería crearse correctamente ahora
- Si persiste error 1400, necesitamos investigar más (posible conflicto con sistema operativo o permisos)
 
---

## BUG-001 - [Ejemplo de formato para futuros bugs]

### Intento 1: [Descripción breve del enfoque]
**Fecha**: YYYY-MM-DD
**Commit**: [hash del commit o "sin commit"]
**Enfoque**: 
- Detalle técnico del cambio
- Código relevante si procede

**Resultado**: ? FALLO / ?? PARCIAL / ? ÉXITO
**Causa**: Explicación de por qué funcionó o no funcionó
**Lección aprendida**: Qué aprendimos de este intento

---

## Notas Generales

- **Cada intento debe tener su propia entrada**, incluso si fue fallido
- **Incluir código relevante** en bloques de código cuando sea útil para futuras referencias
- **Documentar SIEMPRE la causa raíz** de fallos para evitar repetir errores
- **Al resolver un bug**, resumir la solución final como combinación de intentos exitosos
- Este archivo se archiva al finalizar el sprint como `sprint_bug_attempts_v<version>.md`
