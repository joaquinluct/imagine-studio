
# Roadmap - Tareas (sprint activo)

Este fichero contiene las tareas detalladas para cada historia de usuario del sprint activo.

- NOTA: este fichero fue poblado con el backlog inicial. Cada tarea es la unidad mínima implementable y seguirá el proceso de iteración (implementar, doble compilación, commit, actualizar docs).

Tareas iniciales para el sprint (task = unidad mínima implementable):

Tarea 4.02.01 - Crear y grabar un `ID3D12GraphicsCommandList` básico
- Implementar la creación de `ID3D12CommandAllocator` y `ID3D12GraphicsCommandList` por frame, grabar un `ClearRenderTargetView` en el RTV y cerrar la lista.
- Objetivo de la iteración: commitear después de la doble compilación y actualizar `roadmap_tasks.md`.

Tarea 4.02.02 - Ejecutar command lists en `ID3D12CommandQueue` e integrar Present
- Enlazar la llamada a `ExecuteCommandLists` y asegurar que `Present` se llame tras el envío al GPU y la señalización correcta del fence.

Tarea 5.02.01 - Implementar `ID3D12Fence` reciclable por frame y espera en CPU con evento
- Implementar incremento de valor de fence, `SetEventOnCompletion` y reutilizar fence y evento entre frames.

Tarea 8.02.01 - Render target de composición para UI
- Añadir un render target para composición de UI y componer/blitarlo en el render target principal durante `RenderFrame`.

Tarea 6.03.01 - Implementar pool de upload y ruta de staging
- Añadir un pool de buffers de upload y funciones para crear recursos en default heap a partir de datos transmitidos.

Cada tarea debe seguir las reglas de iteración: implementar, ejecutar CMake build (Debug), ejecutar msbuild (/t:Build, Debug), corregir errores hasta que ambas queden limpias, commitear y actualizar `docs/roadmap_log.md` y `docs/commits.md`.


