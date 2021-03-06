#include "HotReloader/TrackKeeper.h"

namespace Duckvil { namespace HotReloader {

    void TrackKeeper::SetObject(void* _pObject)
    {
        m_pObject = _pObject;
    }

    TrackKeeper::TrackKeeper(void* _pObject, RuntimeReflection::__duckvil_resource_type_t _typeHandle) :
        m_pObject(_pObject),
        m_typeHandle(_typeHandle)
    {

    }

    TrackKeeper::~TrackKeeper()
    {

    }

    void* TrackKeeper::GetObject() const
    {
        return m_pObject;
    }

    RuntimeReflection::__duckvil_resource_type_t TrackKeeper::GetTypeHandle() const
    {
        return m_typeHandle;
    }

}}