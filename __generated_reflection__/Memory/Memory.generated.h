#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<IMemory*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_ullCapacity", m_ullCapacity); \
_pSerializer->SerializeProperty("m_ullUsed", m_ullUsed); \
_pSerializer->SerializeProperty("m_ullBlockSize", m_ullBlockSize); \
_pSerializer->SerializeProperty("m_ullBlockSize", m_ullBlockSize); \
_pSerializer->SerializeProperty("m_ullTail", m_ullTail); \
_pSerializer->SerializeProperty("m_ullHead", m_ullHead); \
_pSerializer->SerializeProperty("m_ullBlockSize", m_ullBlockSize); \
_pSerializer->SerializeProperty("m_pHead", m_pHead); \
_pSerializer->SerializeProperty("m_ullBlockSize", m_ullBlockSize); \
_pSerializer->SerializeProperty("m_fnBasicAllocate", m_fnBasicAllocate); \
_pSerializer->SerializeProperty("m_fnCalculateAlignedPointer", m_fnCalculateAlignedPointer); \
_pSerializer->SerializeProperty("m_fnCalculatePadding", m_fnCalculatePadding); \
_pSerializer->SerializeProperty("m_fnCalculatePaddingH", m_fnCalculatePaddingH); \
_pSerializer->SerializeProperty("m_fnCalculateAlignedPointer_", m_fnCalculateAlignedPointer_); \
_pSerializer->SerializeProperty("m_fnCalculatePadding_", m_fnCalculatePadding_); \
_pSerializer->SerializeProperty("m_fnCalculatePaddingH_", m_fnCalculatePaddingH_); \
_pSerializer->SerializeProperty("m_fnLinearAllocate", m_fnLinearAllocate); \
_pSerializer->SerializeProperty("m_fnLinearClear", m_fnLinearClear); \
_pSerializer->SerializeProperty("m_fnFixedStackAllocate_", m_fnFixedStackAllocate_); \
_pSerializer->SerializeProperty("m_fnFixedStackTop_", m_fnFixedStackTop_); \
_pSerializer->SerializeProperty("m_fnFixedStackPop_", m_fnFixedStackPop_); \
_pSerializer->SerializeProperty("m_fnFixedStackEmpty_", m_fnFixedStackEmpty_); \
_pSerializer->SerializeProperty("m_fnFixedStackFull_", m_fnFixedStackFull_); \
_pSerializer->SerializeProperty("m_fnFixedStackClear_", m_fnFixedStackClear_); \
_pSerializer->SerializeProperty("m_fnFixedStackSize_", m_fnFixedStackSize_); \
_pSerializer->SerializeProperty("m_fnFixedStackCapacity_", m_fnFixedStackCapacity_); \
_pSerializer->SerializeProperty("m_fnFixedQueueAllocate_", m_fnFixedQueueAllocate_); \
_pSerializer->SerializeProperty("m_fnFixedQueueBegin_", m_fnFixedQueueBegin_); \
_pSerializer->SerializeProperty("m_fnFixedQueuePop_", m_fnFixedQueuePop_); \
_pSerializer->SerializeProperty("m_fnFixedQueueEmpty_", m_fnFixedQueueEmpty_); \
_pSerializer->SerializeProperty("m_fnFixedQueueFull_", m_fnFixedQueueFull_); \
_pSerializer->SerializeProperty("m_fnFixedQueueClear_", m_fnFixedQueueClear_); \
_pSerializer->SerializeProperty("m_fnFixedQueueSize_", m_fnFixedQueueSize_); \
_pSerializer->SerializeProperty("m_fnFixedQueueCapacity_", m_fnFixedQueueCapacity_); \
_pSerializer->SerializeProperty("m_fnFixedArrayAllocate_", m_fnFixedArrayAllocate_); \
_pSerializer->SerializeProperty("m_fnFixedArrayBegin_", m_fnFixedArrayBegin_); \
_pSerializer->SerializeProperty("m_fnFixedArrayBack_", m_fnFixedArrayBack_); \
_pSerializer->SerializeProperty("m_fnFixedArrayAt_", m_fnFixedArrayAt_); \
_pSerializer->SerializeProperty("m_fnFixedArrayEmpty_", m_fnFixedArrayEmpty_); \
_pSerializer->SerializeProperty("m_fnFixedArrayFull_", m_fnFixedArrayFull_); \
_pSerializer->SerializeProperty("m_fnFixedArraySize_", m_fnFixedArraySize_); \
_pSerializer->SerializeProperty("m_fnFixedArrayClear_", m_fnFixedArrayClear_); \
_pSerializer->SerializeProperty("m_fnFreeListAllocate_", m_fnFreeListAllocate_); \
_pSerializer->SerializeProperty("m_fnFreeListAllocate_D_", m_fnFreeListAllocate_D_); \
_pSerializer->SerializeProperty("m_fnFreeListReallocate_", m_fnFreeListReallocate_); \
_pSerializer->SerializeProperty("m_fnFreeListFree_", m_fnFreeListFree_); \
_pSerializer->SerializeProperty("m_fnFreeListClear_", m_fnFreeListClear_); \
_pSerializer->SerializeProperty("m_fnFixedVectorAllocate_", m_fnFixedVectorAllocate_); \
_pSerializer->SerializeProperty("m_fnFixedVectorBegin_", m_fnFixedVectorBegin_); \
_pSerializer->SerializeProperty("m_fnFixedVectorBack_", m_fnFixedVectorBack_); \
_pSerializer->SerializeProperty("m_fnFixedVectorEnd_", m_fnFixedVectorEnd_); \
_pSerializer->SerializeProperty("m_fnFixedVectorAt_", m_fnFixedVectorAt_); \
_pSerializer->SerializeProperty("m_fnFixedVectorEmpty_", m_fnFixedVectorEmpty_); \
_pSerializer->SerializeProperty("m_fnFixedVectorFull_", m_fnFixedVectorFull_); \
_pSerializer->SerializeProperty("m_fnFixedVectorClear_", m_fnFixedVectorClear_); \
_pSerializer->SerializeProperty("m_fnFixedVectorResize_", m_fnFixedVectorResize_); \
_pSerializer->SerializeProperty("m_fnFixedVectorSize_", m_fnFixedVectorSize_); \
_pSerializer->SerializeProperty("m_fnFixedVectorCapacity_", m_fnFixedVectorCapacity_); \
_pSerializer->SerializeProperty("m_fnFixedVectorErase_", m_fnFixedVectorErase_); \
_pSerializer->SerializeProperty("m_fnLinearAllocateLinearAllocator", m_fnLinearAllocateLinearAllocator); \
_pSerializer->SerializeProperty("m_fnLinearAllocateFixedStackAllocator", m_fnLinearAllocateFixedStackAllocator); \
_pSerializer->SerializeProperty("m_fnLinearAllocateFixedArrayAllocator", m_fnLinearAllocateFixedArrayAllocator); \
_pSerializer->SerializeProperty("m_fnLinearAllocateFreeListAllocator", m_fnLinearAllocateFreeListAllocator); \
_pSerializer->SerializeProperty("m_fnLinearAllocateFixedVectorAllocator", m_fnLinearAllocateFixedVectorAllocator); \
_pSerializer->SerializeProperty("m_fnFreeListAllocateFixedQueueAllocator", m_fnFreeListAllocateFixedQueueAllocator); \
_pSerializer->SerializeProperty("m_fnFreeListAllocateFreeListAllocator", m_fnFreeListAllocateFreeListAllocator); \
_pSerializer->SerializeProperty("m_fnFreeListAllocateFixedVectorAllocator", m_fnFreeListAllocateFixedVectorAllocator); \
_pSerializer->SerializeProperty("m_fnFreeListAllocateFixedStackAllocator", m_fnFreeListAllocateFixedStackAllocator); \
_pSerializer->SerializeProperty("m_fnFreeListAllocateFixedArrayAllocator", m_fnFreeListAllocateFixedArrayAllocator); \
}
