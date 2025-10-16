# Roadmap - Historias de usuario (sprint activo)

Este fichero contiene las historias de usuario que forman el backlog del sprint activo. Cada historia se descompondrá en tareas en `docs/roadmap_tasks.md` y se ejecutará como iteraciones individuales (implementar, doble compilación, commit, actualizar docs).

Historias pendientes para el sprint (backlog inicial):

1) 4.02 Backend DX12: grabado de command lists y ejecución en el dispositivo
- Descripción: grabar un `ID3D12GraphicsCommandList` simple que limpie el render target y ejecutar la lista con `ExecuteCommandLists` en la `ID3D12CommandQueue`. Gestionar correctamente `ID3D12CommandAllocator` y el reset/cierre de las listas.

2) 5.02 Sincronización GPU: sustituir el stub CPU-only Fence por `ID3D12Fence` en rutas de submit/present
- Descripción: asegurar valores de fence correctos, reciclar fences por frame y evitar busy-wait en CPU usando eventos o polling de fence.

3) 8.02 Composición UI vía renderer
- Descripción: implementar la ruta de composición donde la UI renderiza a una textura o render target y el renderer la compone sobre el back buffer antes del `Present`.

4) 6.03 Streaming -> subida a GPU: implementar colas de upload y creación de recursos (texturas/buffers)
- Descripción: construir un pool de upload y una ruta de staging para que los assets transmitidos se suban a memoria GPU de forma segura desde hilos de streaming.

5) 7.02 Estabilización y rendimiento de TaskGraph
- Descripción: endurecer `TaskGraph`, añadir tests y ajustar `ThreadPool` para mayor throughput.

Se añadirán más historias conforme se refine el backlog del sprint.

