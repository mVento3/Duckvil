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

        void* _testT = Duckvil::RuntimeReflection::create(_memoryInterface, _free_list, _rr_data, _t, 10.f);
        float* _prop = (float*)Duckvil::RuntimeReflection::get_property(_rr_data, "m_fA", (Duckvil::Test::TestType*)_testT);
        printf("AAAA\n");
    }

    // {
    //     Duckvil::PlugNPlay::__module_information _parser("Parser");
    //     Duckvil::Parser::__lexer_ftable* (*duckvil_lexer_init)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);
    //     Duckvil::Parser::__ast_ftable* (*duckvil_ast_init)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);

    //     _module.load(&_parser);
    //     _module.get(_parser, "duckvil_lexer_init", (void**)&duckvil_lexer_init);
    //     _module.get(_parser, "duckvil_ast_init", (void**)&duckvil_ast_init);

    //     Duckvil::Parser::__lexer_ftable* _funcs = duckvil_lexer_init(_memoryInterface, _free_list);
    //     Duckvil::Parser::__lexer_data _data;

    //     _funcs->load_file(&_data, (std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "include/Parser/AST.h").string().c_str());

    //     Duckvil::Parser::__ast_ftable* _ast = duckvil_ast_init(_memoryInterface, _free_list);

    //     Duckvil::Parser::__ast _ast_data;

    //     _ast_data.m_aUserDefines.push_back("DUCKVIL_EXPORT");
    //     _ast_data.m_aUserDefines.push_back("slot");
    //     _ast_data.m_aUserDefines.push_back("DUCKVIL_RESOURCE_DECLARE");

    //     _ast->ast_generate(&_ast_data, _funcs, _data);
    //     _ast->ast_print(_ast_data);

    //     {
    //         Duckvil::RuntimeReflection::__generator_ftable* (*duckvil_runtime_reflection_generator)(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);

    //         _module.get(_runtimeReflectionModule, "duckvil_runtime_reflection_generator_init", (void**)&duckvil_runtime_reflection_generator);

    //         Duckvil::RuntimeReflection::__generator_ftable* _generator_ftable = duckvil_runtime_reflection_generator(_memoryInterface, _free_list);
    //         Duckvil::RuntimeReflection::__generator_data* _generatorData = _generator_ftable->init(_memoryInterface, _free_list);

    //         _generator_ftable->generate(_generatorData, (std::filesystem::path(DUCKVIL_OUTPUT).parent_path() / "__generated_reflection__/test.cpp").string().c_str(), _ast_data);
    //     }
    // }

    // duckvil_init(_memoryInterface, _free_list);

    return 0;
}