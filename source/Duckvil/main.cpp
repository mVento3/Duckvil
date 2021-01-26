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

#include "Utils/Benchmark.h"

#include "Duckvil/Duckvil.h"

Duckvil::Utils::CommandArgumentsParser::Descriptor* g_pDescriptors = { 0 };

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::__module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);

    Duckvil::Memory::init_callback duckvil_memory_init;

    _module.get(_memoryModule, "duckvil_memory_init", (void**)&duckvil_memory_init);

    Duckvil::Memory::IMemory* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::__linear_allocator _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024);

    Duckvil::Memory::__free_list_allocator* _free_list = _memoryInterface->m_fnAllocateFreeListAllocator(&_mainMemoryAllocator, 512 * 1024);

    Duckvil::__ftable* _engine = duckvil_init(_memoryInterface, _free_list);
    Duckvil::__data _engineData = {};

    _engine->init(&_engineData, _memoryInterface, _free_list);
    _engine->start(&_engineData, _engine);

    return 0;
}