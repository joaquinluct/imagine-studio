# Roadmap Histories - active sprint

This file lists the user stories (histories) that form the backlog for the active sprint. Each story will be broken down into tasks in `docs/roadmap_tasks.md` and executed as individual iterations (implement, double-build, commit, update docs).

Pending user stories for the sprint (initial backlog):

1) 4.02 DX12 backend: implement command list recording and execute on the device
- Description: record a simple `ID3D12GraphicsCommandList` that clears the render target and integrates `ExecuteCommandLists` on the `ID3D12CommandQueue`. Properly manage `ID3D12CommandAllocator` and command list reset/close semantics.

2) 5.02 GPU sync: replace CPU-only Fence stub with `ID3D12Fence` integration across submit/present paths
- Description: ensure correct fence values, reuse fences per frame, and avoid busy-wait by using events or fence polling.

3) 8.02 UI composition via renderer
- Description: implement composition path where UI draws into a texture or render target and renderer composes it onto the back buffer before present.

4) 6.03 Streaming -> GPU upload: implement upload queues and resource creation for textures/buffers
- Description: build an upload pool and staging buffer path for streamed assets to be uploaded to GPU memory safely across threads.

5) 7.02 TaskGraph stabilization and performance
- Description: harden TaskGraph, add tests, and tune ThreadPool for throughput.

... more stories will be added as the sprint backlog is refined.

