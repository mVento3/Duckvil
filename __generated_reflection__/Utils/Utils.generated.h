#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<string*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_ullLength", m_ullLength); \
_pSerializer->SerializeProperty("m_pMemory", m_pMemory); \
_pSerializer->SerializeProperty("m_pAllocator", m_pAllocator); \
}
