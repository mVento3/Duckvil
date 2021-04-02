#pragma once

#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    struct Container
    {
        Container()
        {
            m_pMemoryInterface = nullptr;
            m_pAllocator = nullptr;
        }

        Container(IMemory* _pMemory, allocator* _pAllocator) :
            m_pMemoryInterface(_pMemory),
            m_pAllocator(_pAllocator)
        {

        }

        IMemory* m_pMemoryInterface;
        allocator* m_pAllocator;
    };

}}