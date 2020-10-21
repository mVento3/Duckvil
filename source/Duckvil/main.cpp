#include <cstdio>
#include <filesystem>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"
#include "PlugNPlay/Plugin.h"

#include "Memory/Vector.h"

#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"

#include "Parser/AST.h"

#include "TestRuntimeReflectionType/Test.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::AutoLoader _auto(DUCKVIL_OUTPUT);

    _auto.LoadAll();

    Duckvil::PlugNPlay::module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);
    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);

    Duckvil::PlugNPlay::__module_information _runtimeReflectionModule("RuntimeReflection");

    _module.load(&_runtimeReflectionModule);

    {
        duckvil_runtime_reflection_init_callback _rr_init;
        duckvil_runtime_reflection_recorder_init_callback _rr_recorder_init;

        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_init", (void**)&_rr_init);
        _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_recorder_init", (void**)&_rr_recorder_init);

        Duckvil::RuntimeReflection::__ftable* _rr_ftable = _rr_init(_memoryInterface, _free_list);
        Duckvil::RuntimeReflection::__recorder_ftable* _rr_recorder = _rr_recorder_init(_memoryInterface, _free_list);

        Duckvil::RuntimeReflection::__data* _rr_data = _rr_ftable->m_fnInit(_memoryInterface, _free_list, _rr_ftable);

        {
            Duckvil::PlugNPlay::__module_information _test_type_module("TestRuntimeReflectionType");

            _module.load(&_test_type_module);

            void (*test_type)(Duckvil::Memory::IMemory* _pMemoryInterface, Duckvil::Memory::__free_list_allocator* _pAllocator, Duckvil::RuntimeReflection::__recorder_ftable* _pRecorder, Duckvil::RuntimeReflection::__ftable* _pRuntimeReflection, Duckvil::RuntimeReflection::__data* _pData);
            uint32_t (*get_recorder_count)();

            _module.get(_test_type_module, "duckvil_get_runtime_reflection_recorder_count", (void**)&get_recorder_count);

            uint32_t _count = get_recorder_count();

            for(uint32_t i = 0; i < _count; i++)
            {
                _module.get(_test_type_module, (std::string("duckvil_runtime_reflection_record_") + std::to_string(i)).c_str(), (void**)&test_type);
            }

            test_type(_memoryInterface, _free_list, _rr_recorder, _rr_ftable, _rr_data);
        }

        Duckvil::RuntimeReflection::__duckvil_resource_type_t _t = _rr_ftable->m_fnGetType(_rr_data, "TestType");
        Duckvil::RuntimeReflection::__duckvil_resource_property_t _propHandle = Duckvil::RuntimeReflection::get_property_handle(_rr_data, _t, "m_fA");

        void* _testT = Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_data, _t, 10.f);
        float* _prop = (float*)Duckvil::RuntimeReflection::get_property(_rr_data, "m_fA", (Duckvil::Test::TestType*)_testT);

        Duckvil::RuntimeReflection::invoke(_rr_data, "test", (Duckvil::Test::TestType*)_testT);
        Duckvil::RuntimeReflection::invoke(_rr_data, "test2", (Duckvil::Test::TestType*)_testT, 10.f, 20.f);
        Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test3");
        Duckvil::RuntimeReflection::invoke<Duckvil::Test::TestType>(_rr_data, "test4", 30.f, 40.f);

        int _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, "a");
        _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, "b");
        _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, 1);
        _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, 1.f);
        _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, _propHandle, 1);
        _res = Duckvil::RuntimeReflection::get_meta_value<int>(_rr_data, _t, _propHandle, "a");

        printf("AAAA\n");
    }

    // duckvil_init(_memoryInterface, _free_list);

    return 0;
}