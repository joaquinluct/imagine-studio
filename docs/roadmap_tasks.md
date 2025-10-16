# Roadmap Tasks

This file will contain the detailed tasks for each user story in the active sprint.

- NOTE: This file is intentionally empty and will be populated as tasks are defined and prioritized.

Initial tasks for the sprint (task = minimal implementable unit):

Task 4.02.01 - Create and record a basic ID3D12GraphicsCommandList
- Implement creation of `ID3D12CommandAllocator` and `ID3D12GraphicsCommandList` per frame, record a `ClearRenderTargetView` on the RTV and close the list.
- Iteration target: commit after double-build and update roadmap_tasks.md.

Task 4.02.02 - Execute command lists on ID3D12CommandQueue and integrate Present
- Wire `ExecuteCommandLists` call and ensure `Present` is called after GPU submission and proper fence signaling.

Task 5.02.01 - Implement recycled ID3D12Fence per frame and CPU event wait
- Implement fence value increment, `SetEventOnCompletion`, reuse fence and event across frames.

Task 8.02.01 - UI composition render target
- Add a render target for UI composition and blit/compose it into the main render target during `RenderFrame`.

Task 6.03.01 - Implement upload buffer pool and staging path
- Add simple upload buffer pool and functions to create default heap resources from streamed data.

Each task must follow the iteration rules: implement, run CMake build (Debug), run msbuild (/t:Build, Debug), fix issues until both clean, commit, and update `docs/roadmap_log.md` and `docs/commits.md`.


