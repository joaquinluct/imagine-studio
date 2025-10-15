#pragma once

#include <cstddef>
#include <vector>
#include <mutex>

namespace Renderer {

class CommandBuffer {
public:
    CommandBuffer();
    ~CommandBuffer();

    // Initialize the command buffer with given capacity in bytes
    void Initialize(size_t capacityBytes);
    void Shutdown();

    // Reset the buffer for new frame
    void Reset();

    // Allocate a region in the ring buffer and return pointer (or nullptr if not enough space)
    void* Allocate(size_t bytes);

    size_t Capacity() const { return capacity_; }

private:
    std::vector<uint8_t> buffer_;
    size_t capacity_ = 0;
    size_t head_ = 0; // write head
    size_t tail_ = 0; // read tail (not used for GPU sync in stub)
    std::mutex mutex_;
};

} // namespace Renderer
