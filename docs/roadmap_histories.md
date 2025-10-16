# Roadmap - Historias de usuario (sprint activo)

Este fichero contiene las historias de usuario que forman el backlog del sprint activo. Cada historia se descompondr� en tareas en `docs/roadmap_tasks.md` y se ejecutar� como iteraciones individuales (implementar, doble compilaci�n, commit, actualizar docs).

Historias pendientes para el sprint (backlog inicial):

1) 4.02 Backend DX12: grabado de command lists y ejecuci�n en el dispositivo
- Descripci�n: grabar un `ID3D12GraphicsCommandList` simple que limpie el render target y ejecutar la lista con `ExecuteCommandLists` en la `ID3D12CommandQueue`. Gestionar correctamente `ID3D12CommandAllocator` y el reset/cierre de las listas.

2) 5.02 Sincronizaci�n GPU: sustituir el stub CPU-only Fence por `ID3D12Fence` en rutas de submit/present
- Descripci�n: asegurar valores de fence correctos, reciclar fences por frame y evitar busy-wait en CPU usando eventos o polling de fence.

3) 8.02 Composici�n UI v�a renderer
- Descripci�n: implementar la ruta de composici�n donde la UI renderiza a una textura o render target y el renderer la compone sobre el back buffer antes del `Present`.

4) 6.03 Streaming -> subida a GPU: implementar colas de upload y creaci�n de recursos (texturas/buffers)
- Descripci�n: construir un pool de upload y una ruta de staging para que los assets transmitidos se suban a memoria GPU de forma segura desde hilos de streaming.

5) 7.02 Estabilizaci�n y rendimiento de TaskGraph
- Descripci�n: endurecer `TaskGraph`, a�adir tests y ajustar `ThreadPool` para mayor throughput.

Se a�adir�n m�s historias conforme se refine el backlog del sprint.

