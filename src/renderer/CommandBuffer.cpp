#include "CommandBuffer.h"

#include <cstring>

namespace Renderer {

CommandBuffer::CommandBuffer() {}
CommandBuffer::~CommandBuffer() {}

void CommandBuffer::Initialize(size_t capacityBytes)
{
    capacity_ = capacityBytes;
    buffer_.assign(capacity_, 0);
    head_ = 0;
    tail_ = 0;
}

unsigned long long CommandBuffer::Submit()
{
    std::lock_guard<std::mutex> lock(mutex_);
    submissionIndex_++;
    // in a real implementation we'd flush to GPU
    return submissionIndex_;
}

void CommandBuffer::ResetForFrame()
{
    Reset();
}

void CommandBuffer::Shutdown()
{
    buffer_.clear();
    capacity_ = 0;
    head_ = tail_ = 0;
}

void CommandBuffer::Reset()
{
    head_ = 0;
    tail_ = 0;
}

void* CommandBuffer::Allocate(size_t bytes)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (bytes == 0 || bytes > capacity_)
        return nullptr;

    if (head_ + bytes <= capacity_)
    {
        void* ptr = buffer_.data() + head_;
        head_ += bytes;
        return ptr;
    }

    // Simple wrap-around (no defragmentation)
    if (bytes <= tail_)
    {
        head_ = bytes;
        return buffer_.data();
    }

    return nullptr; // not enough contiguous space
}

} // namespace Renderer
