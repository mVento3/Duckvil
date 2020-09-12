#include "Memory/Internal/LinearAllocator.h"

#include <memory>

namespace Duckvil { namespace Memory {

    void* linear_allocate(__linear_allocator& _allocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_allocator.capacity < _allocator.used + _ullSize)
        {
            return 0;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_allocator.memory + _allocator.used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _allocator.used += _ullSize + _padding;

        return _memory;
    }

    const char* linear_allocate(__linear_allocator& _allocator, const char* _pData)
    {
        std::size_t _len = strlen(_pData);

        if(_allocator.capacity < _allocator.used + _len)
        {
            return 0;
        }

        void* _memory = (void*)(_allocator.memory + _allocator.used);

        memcpy(_memory, _pData, _len);

        _allocator.used += _len + 1;

        return (const char*)_memory;
    }

    void linear_clear(__linear_allocator& _allocator)
    {
        memset(_allocator.memory, 0, _allocator.capacity);
        _allocator.used = 0;
    }

    void* linear_allocate(__linear_allocator* _pAllocator, const void* _pData, std::size_t _ullSize, uint8_t _ucAlignment)
    {
        if(_pAllocator->capacity < _pAllocator->used + _ullSize)
        {
            return 0;
        }

        uint8_t _padding = 0;
        void* _memory = calculate_aligned_pointer(_pAllocator->memory + _pAllocator->used, _ucAlignment, _padding);

        memcpy(_memory, _pData, _ullSize);

        _pAllocator->used += _ullSize + _padding;

        return _memory;
    }

    const char* linear_allocate(__linear_allocator* _pAllocator, const char* _pData)
    {
        std::size_t _len = strlen(_pData);

        if(_pAllocator->capacity < _pAllocator->used + _len)
        {
            return 0;
        }

        void* _memory = (void*)(_pAllocator->memory + _pAllocator->used);

        memcpy(_memory, _pData, _len);

        _pAllocator->used += _len + 1;

        return (const char*)_memory;
    }

    void linear_clear(__linear_allocator* _pAllocator)
    {
        memset(_pAllocator->memory, 0, _pAllocator->capacity);
        _pAllocator->used = 0;
    }

}}