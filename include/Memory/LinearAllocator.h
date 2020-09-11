#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* linear_allocate(IMemory* _pMemory, __linear_allocator& _allocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnLinearAllocate(_allocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* linear_allocate(IMemory* _pMemory, __linear_allocator& _allocator, const char* _pData)
    {
        return _pMemory->m_fnLinearAllocateCStr(_allocator, _pData);
    }

    template <typename Type>
    Type* linear_allocate(IMemory* _pMemory, __linear_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnLinearAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* linear_allocate(IMemory* _pMemory, __linear_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnLinearAllocateCStr_(_pAllocator, _pData);
    }

    bool linear_empty(__linear_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool linear_full(__linear_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    size_t linear_size(__linear_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    size_t linear_capacity(__linear_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

}}