#pragma once

#include "Memory/Internal/Allocator.h"

#include "Memory/Iterator.h"

#include "Utils/Utils.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    inline Type* fixed_vector_allocate(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedVectorAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    template <std::size_t Length>
    inline const char* fixed_vector_allocate(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator, const char (&_pData)[Length])
    {
        return _pMemory->m_fnFixedVectorAllocateCStr_(_pAllocator, _pData, Length);
    }

    inline void* fixed_vector_begin(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorBegin_(_pAllocator);
    }

    inline void* fixed_vector_back(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorBack_(_pAllocator);
    }

    inline void* fixed_vector_at(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator, std::size_t _ullIndex)
    {
        return _pMemory->m_fnFixedVectorAt_(_pAllocator, _ullIndex);
    }

    inline bool fixed_vector_empty(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorEmpty_(_pAllocator);
    }

    inline bool fixed_vector_full(IMemory* _pMemory, __fixed_vector_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedVectorFull_(_pAllocator);
    }

    inline void fixed_vector_resize(IMemory* _pMemory, __free_list_allocator* m_pParentAllocator, __fixed_vector_allocator** _pAllocator, std::size_t _ullNewSize)
    {
        _pMemory->m_fnFixedVectorResize_(_pMemory, m_pParentAllocator, _pAllocator, _ullNewSize);
    }

}}