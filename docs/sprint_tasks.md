
# Sprint v1.1.0 - Tareas Detalladas

> **Referencia**: Ver [`docs/MAIN.md`](MAIN.md) para los pilares fundamentales del proyecto.

Este fichero contiene las tareas detalladas para cada historia de usuario del sprint activo.

**NOTA**: Cada tarea es la unidad mínima implementable y seguirá el proceso de iteración (implementar, doble compilación, commit, actualizar docs).

---

## ?? Tareas por Historia

### **Historia H1 - Inicialización DX12** ??

#### **T1.1 - Crear DX12 Device y DXGI Factory**
**ID**: T1.1  
**Historia**: H1  
**Descripción**: Implementar la creación de `IDXGIFactory4` (usando `CreateDXGIFactory2`), enumerar adaptadores GPU, y crear `ID3D12Device` con el adaptador seleccionado.

**Implementación**:
```cpp
// En DX12Device.cpp:
- IDXGIFactory4* factory;
- CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));
- IDXGIAdapter1* adapter;
- factory->EnumAdapters1(0, &adapter); // Seleccionar primer adapter
- D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
```

**Archivos afectados**: `src/renderer/DX12Device.h`, `src/renderer/DX12Device.cpp`

**Criterio de éxito**: Device creado sin errores, logs confirman GPU seleccionada.

---

#### **T1.2 - Crear Command Queue**
**ID**: T1.2  
**Historia**: H1  
**Descripción**: Crear `ID3D12CommandQueue` de tipo DIRECT para enviar comandos a la GPU.

**Implementación**:
```cpp
// En DX12Device.cpp:
D3D12_COMMAND_QUEUE_DESC queueDesc = {};
queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
```

**Archivos afectados**: `src/renderer/DX12Device.h`, `src/renderer/DX12Device.cpp`

**Criterio de éxito**: Command queue creada correctamente.

---

#### **T1.3 - Crear SwapChain con Back Buffers**
**ID**: T1.3  
**Historia**: H1  
**Descripción**: Crear `IDXGISwapChain3` con 2-3 back buffers usando `CreateSwapChainForHwnd`, obtener buffers con `GetBuffer` y crear RTVs.

**Implementación**:
```cpp
// En DX12Renderer.cpp:
DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
swapChainDesc.Width = width;
swapChainDesc.Height = height;
swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
swapChainDesc.BufferCount = 2;
swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
swapChainDesc.SampleDesc.Count = 1;

factory->CreateSwapChainForHwnd(
    commandQueue, hwnd, &swapChainDesc, 
    nullptr, nullptr, &swapChain1
);
swapChain1->QueryInterface(&m_swapChain); // IDXGISwapChain3

// Obtener back buffers y crear RTVs
for (UINT i = 0; i < 2; ++i) {
    m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
    device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);
}
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: SwapChain creado, 2 back buffers obtenidos y RTVs creados.

---

#### **T1.4 - Crear Descriptor Heaps (RTV, CBV/SRV/UAV)**
**ID**: T1.4  
**Historia**: H1  
**Descripción**: Crear descriptor heaps para Render Target Views (RTV) y para Constant/Shader/Unordered Access Views (CBV/SRV/UAV).

**Implementación**:
```cpp
// En DX12Device.cpp:
D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
rtvHeapDesc.NumDescriptors = 2; // 2 back buffers
rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
cbvHeapDesc.NumDescriptors = 1; // Para constant buffer
cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap));
```

**Archivos afectados**: `src/renderer/DX12Device.h`, `src/renderer/DX12Device.cpp`

**Criterio de éxito**: Descriptor heaps creados y handles obtenidos.

---

#### **T1.5 - Crear Command Allocators, Command List y Fence GPU**
**ID**: T1.5  
**Historia**: H1  
**Descripción**: Crear `ID3D12CommandAllocator` por frame, `ID3D12GraphicsCommandList`, `ID3D12Fence` y evento Win32 para sincronización.

**Implementación**:
```cpp
// En DX12Renderer.cpp:
// Command Allocator (uno por frame)
m_device->CreateCommandAllocator(
    D3D12_COMMAND_LIST_TYPE_DIRECT, 
    IID_PPV_ARGS(&m_commandAllocator)
);

// Command List
m_device->CreateCommandList(
    0, 
    D3D12_COMMAND_LIST_TYPE_DIRECT, 
    m_commandAllocator, 
    nullptr, 
    IID_PPV_ARGS(&m_commandList)
);
m_commandList->Close(); // Inicia cerrada

// Fence GPU
m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
m_fenceValue = 1;
m_fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Command allocator, list, fence y evento creados sin errores.

---

### **Historia H2 - Pipeline State & Shaders** ??

#### **T2.1 - Crear Root Signature**
**ID**: T2.1  
**Historia**: H2  
**Descripción**: Definir y crear Root Signature con root constants para MVP matrix (16 floats = 64 bytes).

**Implementación**:
```cpp
// En DX12Renderer.cpp:
D3D12_ROOT_PARAMETER rootParam = {};
rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
rootParam.Constants.ShaderRegister = 0; // register(b0)
rootParam.Constants.Num32BitValues = 16; // 4x4 matrix
rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
rootSigDesc.NumParameters = 1;
rootSigDesc.pParameters = &rootParam;
rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

ID3DBlob* signature;
ID3DBlob* error;
D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Root Signature creada y serializada correctamente.

---

#### **T2.2 - Compilar Shaders HLSL (Vertex & Pixel)**
**ID**: T2.2  
**Historia**: H2  
**Descripción**: Crear archivos HLSL para vertex y pixel shaders, compilarlos con `D3DCompileFromFile` o runtime compilation.

**Implementación**:
```hlsl
// shaders/quad.hlsl:
cbuffer Constants : register(b0) {
    float4x4 mvp;
};

struct VSInput {
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct PSInput {
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

PSInput VSMain(VSInput input) {
    PSInput output;
    output.pos = mul(float4(input.pos, 1.0), mvp);
    output.col = input.col;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET {
    return input.col;
}
```

```cpp
// En DX12Renderer.cpp:
ID3DBlob* vsBlob;
ID3DBlob* psBlob;
D3DCompileFromFile(L"shaders/quad.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vsBlob, nullptr);
D3DCompileFromFile(L"shaders/quad.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &psBlob, nullptr);
```

**Archivos afectados**: `shaders/quad.hlsl` (nuevo), `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Shaders compilados sin errores, bytecode listo.

---

#### **T2.3 - Crear Pipeline State Object (PSO)**
**ID**: T2.3  
**Historia**: H2  
**Descripción**: Crear PSO con InputLayout (POSITION, COLOR), shaders compilados y estados de render.

**Implementación**:
```cpp
// En DX12Renderer.cpp:
D3D12_INPUT_ELEMENT_DESC inputElements[] = {
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};

D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
psoDesc.InputLayout = {inputElements, 2};
psoDesc.pRootSignature = m_rootSignature;
psoDesc.VS = {vsBlob->GetBufferPointer(), vsBlob->GetBufferSize()};
psoDesc.PS = {psBlob->GetBufferPointer(), psBlob->GetBufferSize()};
psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
psoDesc.DepthStencilState.DepthEnable = FALSE;
psoDesc.SampleMask = UINT_MAX;
psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
psoDesc.NumRenderTargets = 1;
psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
psoDesc.SampleDesc.Count = 1;

m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: PSO creado sin errores.

---

### **Historia H3 - Geometry & Resources** ??

#### **T3.1 - Crear Vertex Buffer (Upload + Default Heap)**
**ID**: T3.1  
**Historia**: H3  
**Descripción**: Crear vertex buffer con 6 vértices (quad = 2 triángulos), upload vía staging heap y copy a default heap.

**Implementación**:
```cpp
// En DX12Renderer.cpp:
struct Vertex {
    float pos[3];
    float col[4];
};

Vertex vertices[] = {
    {{-0.5f, -0.5f, 0.0f}, {1,0,0,1}},
    {{ 0.5f, -0.5f, 0.0f}, {0,1,0,1}},
    {{-0.5f,  0.5f, 0.0f}, {0,0,1,1}},
    {{ 0.5f, -0.5f, 0.0f}, {0,1,0,1}},
    {{ 0.5f,  0.5f, 0.0f}, {1,1,0,1}},
    {{-0.5f,  0.5f, 0.0f}, {0,0,1,1}},
};

// Upload heap
CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);
CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));
m_device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));

// Map y copy
void* pData;
uploadBuffer->Map(0, nullptr, &pData);
memcpy(pData, vertices, sizeof(vertices));
uploadBuffer->Unmap(0, nullptr);

// Default heap
CD3DX12_HEAP_PROPERTIES defaultHeap(D3D12_HEAP_TYPE_DEFAULT);
m_device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

// Copy command
commandList->CopyBufferRegion(m_vertexBuffer, 0, uploadBuffer, 0, sizeof(vertices));

// Barrier
CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
commandList->ResourceBarrier(1, &barrier);
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Vertex buffer en GPU con datos correctos.

---

#### **T3.2 - Crear Constant Buffer (MVP Matrix)**
**ID**: T3.2  
**Historia**: H3  
**Descripción**: Crear constant buffer con MVP matrix (identity para "Hola Mundo"), mapeado para updates.

**Implementación**:
```cpp
// En DX12Renderer.cpp:
float mvp[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}; // Identity

CD3DX12_HEAP_PROPERTIES uploadHeap(D3D12_HEAP_TYPE_UPLOAD);
CD3DX12_RESOURCE_DESC cbDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(mvp) + 255) & ~255); // 256-byte aligned
m_device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &cbDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_constantBuffer));

// Map (mantener mapeado)
m_constantBuffer->Map(0, nullptr, &m_cbMappedData);
memcpy(m_cbMappedData, mvp, sizeof(mvp));
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Constant buffer creado y mapeado.

---

#### **T3.3 - Configurar Vertex Buffer View**
**ID**: T3.3  
**Historia**: H3  
**Descripción**: Configurar `D3D12_VERTEX_BUFFER_VIEW` para bind del vertex buffer en command list.

**Implementación**:
```cpp
// En DX12Renderer.cpp:
m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
m_vertexBufferView.StrideInBytes = sizeof(Vertex); // 28 bytes
m_vertexBufferView.SizeInBytes = sizeof(vertices); // 6 * 28 = 168
```

**Archivos afectados**: `src/renderer/DX12Renderer.h`, `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Vertex buffer view configurado correctamente.

---

### **Historia H4 - Command Recording & Execution** ??

#### **T4.1 - Grabar Command List (Clear + Draw Call)**
**ID**: T4.1  
**Historia**: H4  
**Descripción**: Implementar grabación de command list con clear de RTV y draw call `DrawInstanced`.

**Implementación**:
```cpp
// En DX12Renderer::RenderFrame():
m_commandAllocator->Reset();
m_commandList->Reset(m_commandAllocator, m_pipelineState);

// Set root signature
m_commandList->SetGraphicsRootSignature(m_rootSignature);

// Viewport y scissor
D3D12_VIEWPORT viewport = {0, 0, (float)width, (float)height, 0, 1};
D3D12_RECT scissor = {0, 0, (LONG)width, (LONG)height};
m_commandList->RSSetViewports(1, &viewport);
m_commandList->RSSetScissorRects(1, &scissor);

// Barrier: PRESENT -> RENDER_TARGET
CD3DX12_RESOURCE_BARRIER barrier1 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
m_commandList->ResourceBarrier(1, &barrier1);

// RTV
CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, m_rtvDescriptorSize);
m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

// Clear
float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

// Set topology
m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

// Set vertex buffer
m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

// Set constant buffer (root constants)
m_commandList->SetGraphicsRoot32BitConstants(0, 16, mvp, 0);

// DRAW CALL
m_commandList->DrawInstanced(6, 1, 0, 0);

// Barrier: RENDER_TARGET -> PRESENT
CD3DX12_RESOURCE_BARRIER barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
m_commandList->ResourceBarrier(1, &barrier2);

// Close
m_commandList->Close();
```

**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Command list grabada sin errores, incluye clear y draw call.

---

#### **T4.2 - Ejecutar Command Queue**
**ID**: T4.2  
**Historia**: H4  
**Descripción**: Enviar command list a command queue para ejecución en GPU.

**Implementación**:
```cpp
// En DX12Renderer::RenderFrame():
ID3D12CommandList* ppCommandLists[] = {m_commandList};
m_commandQueue->ExecuteCommandLists(1, ppCommandLists);
```

**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Command queue ejecuta command lists sin errores.

---

#### **T4.3 - Implementar Fence GPU y Sincronización CPU/GPU**
**ID**: T4.3  
**Historia**: H4  
**Descripción**: Señalizar fence tras ejecutar comandos y esperar en CPU usando evento Win32.

**Implementación**:
```cpp
// En DX12Renderer::RenderFrame():
const UINT64 fenceValueNow = m_fenceValue;
m_commandQueue->Signal(m_fence, fenceValueNow);
m_fenceValue++;

// Wait si GPU no ha terminado
if (m_fence->GetCompletedValue() < fenceValueNow) {
    m_fence->SetEventOnCompletion(fenceValueNow, m_fenceEvent);
    WaitForSingleObject(m_fenceEvent, INFINITE);
}
```

**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Fence sincroniza CPU/GPU correctamente, sin deadlocks.

---

#### **T4.4 - Implementar Present con VSync**
**ID**: T4.4  
**Historia**: H4  
**Descripción**: Llamar a `Present(1, 0)` para mostrar el frame con VSync habilitado (60 FPS).

**Implementación**:
```cpp
// En DX12Renderer::RenderFrame():
m_swapChain->Present(1, 0); // VSync habilitado
m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
```

**Archivos afectados**: `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**: Present ejecutado correctamente, ventana muestra quad renderizado, 60 FPS con VSync.

---

## ?? Resumen de Tareas

| ID | Título | Historia | Prioridad | Estado |
|----|--------|----------|-----------|--------|
| T1.1 | Crear DX12 Device y DXGI Factory | H1 | ?? CRÍTICO | ? Pendiente |
| T1.2 | Crear Command Queue | H1 | ?? CRÍTICO | ? Pendiente |
| T1.3 | Crear SwapChain con Back Buffers | H1 | ?? CRÍTICO | ? Pendiente |
| T1.4 | Crear Descriptor Heaps | H1 | ?? CRÍTICO | ? Pendiente |
| T1.5 | Crear Command Allocators, List y Fence | H1 | ?? CRÍTICO | ? Pendiente |
| T2.1 | Crear Root Signature | H2 | ?? ALTO | ? Pendiente |
| T2.2 | Compilar Shaders HLSL | H2 | ?? ALTO | ? Pendiente |
| T2.3 | Crear Pipeline State Object | H2 | ?? ALTO | ? Pendiente |
| T3.1 | Crear Vertex Buffer | H3 | ?? MEDIO | ? Pendiente |
| T3.2 | Crear Constant Buffer | H3 | ?? MEDIO | ? Pendiente |
| T3.3 | Configurar Vertex Buffer View | H3 | ?? MEDIO | ? Pendiente |
| T4.1 | Grabar Command List | H4 | ?? BÁSICO | ? Pendiente |
| T4.2 | Ejecutar Command Queue | H4 | ?? BÁSICO | ? Pendiente |
| T4.3 | Implementar Fence GPU | H4 | ?? BÁSICO | ? Pendiente |
| T4.4 | Implementar Present con VSync | H4 | ?? BÁSICO | ? Pendiente |

**Total**: 15 tareas (pasos pequeños, cada uno es una iteración completa)

---

## ?? Proceso de Ejecución de Tareas

Cada tarea debe seguir las reglas de iteración:
1. **Implementar** el código según la descripción
2. **Compilar CMake** (`cmake --build build --config Debug`)
3. **Compilar MSBuild** (`msbuild "build/ImagineStudio.sln" /t:Build /p:Configuration=Debug /m`)
4. **Corregir errores** hasta que ambas queden limpias (0 errores, 0 warnings)
5. **Commit** y actualizar `docs/daily.md` y `docs/commits.md`

---

## ?? Notas

- Para consultar tareas completadas, ver los archivos versionados `sprint_tasks_v*.md`.
- Las tareas se derivan de las historias en `docs/sprint_histories.md`.
- **Orden de ejecución**: T1.1 ? T1.2 ? T1.3 ? T1.4 ? T1.5 ? T2.1 ? T2.2 ? T2.3 ? T3.1 ? T3.2 ? T3.3 ? T4.1 ? T4.2 ? T4.3 ? T4.4
- Cada tarea es **independiente y compilable** al finalizar (enfoque incremental AAA).


