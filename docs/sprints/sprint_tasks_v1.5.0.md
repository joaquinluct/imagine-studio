# Sprint Tasks - v1.5.0 (Viewport Real)

> **Referencia**: Ver [`docs/sprint_histories.md`](sprint_histories.md) para historias de usuario detalladas.

---

## 📋 Tareas Granulares (Unit of Work)

### **H1 - Render Target como Texture**

#### **H1.1 - Crear Descriptor Heap para SRV**

**ID**: H1.1  
**Historia**: H1  
**Prioridad**: CRÍTICA  
**Complejidad**: Media

**Descripción**:  
Crear descriptor heap para SRVs (Shader Resource Views) si no existe. Este heap almacenará descriptors que apuntan a texturas que ImGui puede usar.

**Implementación**:
```cpp
// src/renderer/DX12Renderer.h
class DX12Renderer {
    // ...existing code...
    
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    UINT m_srvDescriptorSize;
    
    void CreateSRVDescriptorHeap();
    // ...existing code...
};

// src/renderer/DX12Renderer.cpp
void DX12Renderer::CreateSRVDescriptorHeap()
{
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1; // Solo necesitamos 1 para render target
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    
    HRESULT hr = m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));
    if (FAILED(hr)) {
        CORE_LOG_ERROR("Failed to create SRV descriptor heap");
        return;
    }
    
    m_srvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CORE_LOG_INFO("SRV descriptor heap created");
}
```

**Archivos afectados**:
- `src/renderer/DX12Renderer.h`
- `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**:
- Descriptor heap creado sin errores
- `m_srvHeap` != nullptr
- Compilación limpia (CMake + MSBuild)

---

#### **H1.2 - Crear SRV Descriptor para Render Target**

**ID**: H1.2  
**Historia**: H1  
**Prioridad**: CRÍTICA  
**Complejidad**: Media

**Descripción**:  
Crear SRV descriptor que apunte al back buffer (render target) actual. Este descriptor permite que ImGui acceda al render target como textura.

**Implementación**:
```cpp
// src/renderer/DX12Renderer.h
class DX12Renderer {
    // ...existing code...
    
    D3D12_CPU_DESCRIPTOR_HANDLE m_renderTargetSRV_CPU;
    D3D12_GPU_DESCRIPTOR_HANDLE m_renderTargetSRV_GPU;
    
    void CreateRenderTargetSRV();
    D3D12_GPU_DESCRIPTOR_HANDLE GetRenderTargetSRV() const { return m_renderTargetSRV_GPU; }
    // ...existing code...
};

// src/renderer/DX12Renderer.cpp
void DX12Renderer::CreateRenderTargetSRV()
{
    m_renderTargetSRV_CPU = m_srvHeap->GetCPUDescriptorHandleForHeapStart();
    m_renderTargetSRV_GPU = m_srvHeap->GetGPUDescriptorHandleForHeapStart();
    
    // Crear SRV para el back buffer actual
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = 1;
    
    m_device->CreateShaderResourceView(
        m_renderTargets[m_currentBackBuffer].Get(),
        &srvDesc,
        m_renderTargetSRV_CPU
    );
    
    CORE_LOG_INFO("Render target SRV created");
}
```

**Archivos afectados**:
- `src/renderer/DX12Renderer.h`
- `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**:
- SRV descriptor creado sin errores
- `GetRenderTargetSRV()` retorna handle válido
- Compilación limpia (CMake + MSBuild)

---

#### **H1.3 - Implementar Resource State Transitions**

**ID**: H1.3  
**Historia**: H1  
**Prioridad**: CRÍTICA  
**Complejidad**: Alta

**Descripción**:  
Implementar transiciones de estado correctas para el render target:
- `RENDER_TARGET` → `PIXEL_SHADER_RESOURCE` antes de usar como textura en ImGui
- `PIXEL_SHADER_RESOURCE` → `PRESENT` antes de mostrar frame

**Implementación**:
```cpp
// src/renderer/DX12Renderer.cpp

void DX12Renderer::Present()
{
    // ...existing code (render scene)...
    
    // Transición: RENDER_TARGET → PIXEL_SHADER_RESOURCE (para ImGui)
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_renderTargets[m_currentBackBuffer].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    
    m_commandList->ResourceBarrier(1, &barrier);
    
    // Aquí ImGui puede usar el render target como textura
    
    // Transición: PIXEL_SHADER_RESOURCE → PRESENT (para mostrar)
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    
    m_commandList->ResourceBarrier(1, &barrier);
    
    // ...existing code (ExecuteCommandLists, Present)...
}
```

**Archivos afectados**:
- `src/renderer/DX12Renderer.cpp`

**Criterio de éxito**:
- Transiciones de estado sin errores de validación layer
- ImGui puede leer el render target como textura
- Compilación limpia (CMake + MSBuild)

---

#### **H1.4 - Validación Visual SRV**

**ID**: H1.4  
**Historia**: H1  
**Prioridad**: CRÍTICA  
**Complejidad**: Baja

**Descripción**:  
Validar que el SRV del render target se puede usar en ImGui sin artefactos visuales.

**Archivos afectados**:
- `src/editor/EditorUI.cpp`

**Criterio de éxito**:
- Viewport muestra render 3D (no placeholder texto)
- Sin artefactos visuales (flickering, tearing)
- Compilación limpia (CMake + MSBuild)

---

### **H2 - Camera System**

#### **H2.1 - Camera Class (Core)**

**ID**: H2.1  
**Historia**: H2  
**Prioridad**: CRÍTICA  
**Complejidad**: Media

**Descripción**:  
Implementar clase `Camera` con propiedades básicas y métodos para obtener matrices view/projection.

**Archivos afectados**:
- `src/scene/Camera.h` (nuevo)
- `src/scene/Camera.cpp` (nuevo)

**Criterio de éxito**:
- Camera class compila sin errores
- GetViewMatrix() retorna matriz correcta
- GetProjectionMatrix() retorna matriz correcta
- Compilación limpia (CMake + MSBuild)

---

#### **H2.2 - Camera Controls (Orbit, Pan, Zoom)**

**ID**: H2.2  
**Historia**: H2  
**Prioridad**: CRÍTICA  
**Complejidad**: Alta

**Descripción**:  
Implementar métodos de control de cámara.

**Archivos afectados**:
- `src/scene/Camera.h`
- `src/scene/Camera.cpp`

**Criterio de éxito**:
- Orbit rota cámara alrededor del target
- Pan mueve cámara lateralmente
- Zoom acerca/aleja con clamp
- Compilación limpia (CMake + MSBuild)

---

#### **H2.3 - Integración con InputManager**

**ID**: H2.3  
**Historia**: H2  
**Prioridad**: ALTA  
**Complejidad**: Media

**Descripción**:  
Integrar controles de cámara con mouse.

**Archivos afectados**:
- `src/editor/EditorUI.cpp`

**Criterio de éxito**:
- Mouse derecho drag → orbit
- Mouse medio drag → pan
- Mouse scroll → zoom
- Compilación limpia (CMake + MSBuild)

---

#### **H2.4 - Tests Unitarios Camera**

**ID**: H2.4  
**Historia**: H2  
**Prioridad**: ALTA  
**Complejidad**: Media

**Descripción**:  
Crear tests unitarios para Camera.

**Archivos afectados**:
- `tests/camera_test.cpp` (nuevo)

**Criterio de éxito**:
- 15+ tests PASSED
- Compilación limpia (CMake + MSBuild)

---

### **H3 - Viewport Integration**

#### **H3.1 - RenderViewport con ImGui::Image**

**ID**: H3.1  
**Historia**: H3  
**Prioridad**: CRÍTICA  
**Complejidad**: Baja

**Descripción**:  
Actualizar RenderViewport para usar ImGui::Image con SRV.

**Archivos afectados**:
- `src/editor/EditorUI.cpp`

**Criterio de éxito**:
- Viewport muestra render 3D
- Compilación limpia (CMake + MSBuild)

---

#### **H3.2 - Resize Dinámico del Render Target**

**ID**: H3.2  
**Historia**: H3  
**Prioridad**: CRÍTICA  
**Complejidad**: Alta

**Descripción**:  
Implementar resize dinámico del render target.

**Archivos afectados**:
- `src/renderer/DX12Renderer.h/cpp`
- `src/editor/EditorUI.cpp`

**Criterio de éxito**:
- Resize funciona sin errores
- Aspect ratio correcto
- Compilación limpia (CMake + MSBuild)

---

#### **H3.3 - Integrar Camera con Scene**

**ID**: H3.3  
**Historia**: H3  
**Prioridad**: ALTA  
**Complejidad**: Media

**Descripción**:  
Integrar Camera en Scene.

**Archivos afectados**:
- `src/scene/Scene.h/cpp`

**Criterio de éxito**:
- Scene tiene Camera funcional
- Compilación limpia (CMake + MSBuild)

---

#### **H3.4 - Validación Visual Viewport**

**ID**: H3.4  
**Historia**: H3  
**Prioridad**: ALTA  
**Complejidad**: Baja

**Descripción**:  
Validar visualmente que el Viewport funciona.

**Criterio de éxito**:
- Viewport muestra entities en 3D
- Controles funcionan
- Performance > 200 FPS
- Validación usuario: OK

---

### **H4 - Picking & Selection**

#### **H4.1 - Screen to World Ray**

**ID**: H4.1  
**Historia**: H4  
**Prioridad**: ALTA  
**Complejidad**: Alta

**Descripción**:  
Implementar conversión screen coords a ray.

**Archivos afectados**:
- `src/scene/Raycast.h` (nuevo)
- `src/scene/Raycast.cpp` (nuevo)

**Criterio de éxito**:
- ScreenToWorldRay retorna ray válido
- Compilación limpia (CMake + MSBuild)

---

#### **H4.2 - Ray-AABB Intersection**

**ID**: H4.2  
**Historia**: H4  
**Prioridad**: ALTA  
**Complejidad**: Alta

**Descripción**:  
Implementar algoritmo ray-AABB.

**Archivos afectados**:
- `src/scene/Raycast.h`
- `src/scene/Raycast.cpp`

**Criterio de éxito**:
- IntersectRayAABB funciona correctamente
- Compilación limpia (CMake + MSBuild)

---

#### **H4.3 - Scene::PickEntity + Integración Editor**

**ID**: H4.3  
**Historia**: H4  
**Prioridad**: ALTA  
**Complejidad**: Media

**Descripción**:  
Implementar picking y selección desde viewport.

**Archivos afectados**:
- `src/scene/Scene.h/cpp`
- `src/editor/EditorUI.cpp`
- `tests/raycast_test.cpp` (nuevo)

**Criterio de éxito**:
- Click selecciona entity
- 10+ tests PASSED
- Validación usuario: picking funciona

---

## 📊 Resumen de Tareas

**Total**: 15 tareas  
**Completadas**: 0  
**Progreso**: 0.0%

---

**Versión**: v1.0  
**Última actualización**: 2025-01-18  
**Sprint**: v1.5.0 (Viewport Real)
