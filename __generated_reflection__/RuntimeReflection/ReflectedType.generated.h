#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<ReflectedType*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_pReflectionData", m_pReflectionData); \
_pSerializer->SerializeProperty("m_pReflection", m_pReflection); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
_pSerializer->SerializeProperty("m_pReflectionData", m_pReflectionData); \
_pSerializer->SerializeProperty("m_pReflection", m_pReflection); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_typeHandle", m_typeHandle); \
}
