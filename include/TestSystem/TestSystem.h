#pragma once

#include "Duckvil/ISystem.h"
#include "Duckvil/ReflectionFlags.h"

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Recorder.h"

#include "Memory/FreeList.h"

#include "Event/Pool.h"

#include "HotReloader/HotObject.h"

#include "TestSystem/TestSystem.generated.h"

namespace Duckvil { namespace LOL {

    struct test
    {
        int a;
    };

    struct test2
    {
        int a;
    };

    struct aaa
    {
        void OnEvent(const test& _event)
        {
            printf("NOOOOOOO\n");
        }
    };

    struct aaa2
    {
        void OnEvent(const test& _event)
        {
            printf("NOOOOOOO2\n");
        }
    };

    DUCKVIL_CLASS(ReflectionFlags::ReflectionFlags_EngineSystem)
    class Test : public ISystem, public RuntimeCompiler::HotObject
    {
        DUCKVIL_GENERATED_BODY
    private:
        int m_iA;

    public:
        Test();
        Test(const Memory::FreeList& _heap, RuntimeReflection::__data* _pReflectionData, RuntimeReflection::__recorder_ftable* _pReflectionRecorderFTable, RuntimeReflection::__ftable* _pReflectionFTable);
        ~Test();

        bool Init();
        void Update();

        void OnEvent(const test& _event);
        void OnEvent(const test2& _event);
    };

}}