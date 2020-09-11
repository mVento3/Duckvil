#pragma once

#include "Memory/Internal/Allocator.h"

namespace Duckvil { namespace Memory {

    template <typename Type>
    Type* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const Type& _pData)
    {
        return (Type*)_pMemory->m_fnFixedStackAllocate_(_pAllocator, &_pData, sizeof(Type), alignof(Type));
    }

    const char* fixed_stack_allocate(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator, const char* _pData)
    {
        return _pMemory->m_fnFixedStackAllocateCStr_(_pAllocator, _pData);
    }

    void* fixed_stack_top(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator)
    {
        return _pMemory->m_fnFixedStackTop_(_pAllocator);
    }

    void fixed_stack_pop(IMemory* _pMemory, __fixed_stack_allocator* _pAllocator)
    {
        _pMemory->m_fnFixedStackPop_(_pAllocator);
    }

    bool fixed_stack_empty(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == 0;
    }

    bool fixed_stack_full(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used == _pAllocator->capacity;
    }

    size_t fixed_stack_size(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->used;
    }

    size_t fixed_stack_capacity(__fixed_stack_allocator* _pAllocator)
    {
        return _pAllocator->capacity;
    }

    template <typename Type>
    class FixedStack
    {
    private:
        __fixed_stack_allocator* m_pAllocator;
        IMemory* m_pMemoryInterface;

    public:
        FixedStack(IMemory* _pMemoryInterface, __allocator* _pAllocator, size_t _ullCount) :
            m_pMemoryInterface(_pMemoryInterface)
        {
            m_pAllocator = m_pMemoryInterface->m_fnAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        ~FixedStack()
        {

        }

        Type* Allocate(const Type& _data)
        {
            return fixed_stack_allocate(m_pMemoryInterface, m_pAllocator, _data);
        }

        const Type& Top() const
        {
            return *(Type*)fixed_stack_top(m_pMemoryInterface, m_pAllocator);
        }

        void Pop()
        {
            fixed_stack_pop(m_pMemoryInterface, m_pAllocator);
        }

        bool Empty() const
        {
            return fixed_stack_empty(m_pAllocator);
        }

        bool Full() const
        {
            return fixed_stack_full(m_pAllocator);
        }
    };

}}