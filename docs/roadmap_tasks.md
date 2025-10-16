
# Roadmap - Tareas (sprint activo)

Este fichero contiene las tareas detalladas para cada historia de usuario del sprint activo.

- NOTA: este fichero fue poblado con el backlog inicial. Cada tarea es la unidad m�nima implementable y seguir� el proceso de iteraci�n (implementar, doble compilaci�n, commit, actualizar docs).

Tareas iniciales para el sprint (task = unidad m�nima implementable):

Tarea 4.02.01 - Crear y grabar un `ID3D12GraphicsCommandList` b�sico
- Implementar la creaci�n de `ID3D12CommandAllocator` y `ID3D12GraphicsCommandList` por frame, grabar un `ClearRenderTargetView` en el RTV y cerrar la lista.
- Objetivo de la iteraci�n: commitear despu�s de la doble compilaci�n y actualizar `roadmap_tasks.md`.

Tarea 4.02.02 - Ejecutar command lists en `ID3D12CommandQueue` e integrar Present
- Enlazar la llamada a `ExecuteCommandLists` y asegurar que `Present` se llame tras el env�o al GPU y la se�alizaci�n correcta del fence.

Tarea 5.02.01 - Implementar `ID3D12Fence` reciclable por frame y espera en CPU con evento
- Implementar incremento de valor de fence, `SetEventOnCompletion` y reutilizar fence y evento entre frames.

Tarea 8.02.01 - Render target de composici�n para UI
- A�adir un render target para composici�n de UI y componer/blitarlo en el render target principal durante `RenderFrame`.

Tarea 6.03.01 - Implementar pool de upload y ruta de staging
- A�adir un pool de buffers de upload y funciones para crear recursos en default heap a partir de datos transmitidos.

Cada tarea debe seguir las reglas de iteraci�n: implementar, ejecutar CMake build (Debug), ejecutar msbuild (/t:Build, Debug), corregir errores hasta que ambas queden limpias, commitear y actualizar `docs/roadmap_log.md` y `docs/commits.md`.


