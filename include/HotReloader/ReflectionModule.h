#pragma once

#include "RuntimeReflection/Markers.h"
#include "RuntimeReflection/Generator.h"
#include "RuntimeReflection/RuntimeReflection.h"

#include "Engine/ReflectionFlags.h"

#include "Parser/AST.h"

#include "Memory/FreeList.h"

#include <fstream>

#include "Serializer/Runtime/ISerializer.h"

#include "Engine/ISystem.h"

namespace Duckvil { namespace HotReloader {

    class RuntimeCompilerReflectionModule;

    void recursive(RuntimeCompilerReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity);

    DUCKVIL_CLASS(Duckvil::ReflectionFlags::ReflectionFlags_ReflectionModule)
    class RuntimeCompilerReflectionModule
    {
    private:

    public:
        RuntimeCompilerReflectionModule(const Memory::FreeList& _heap, RuntimeReflection::__ftable* _pReflection, RuntimeReflection::__data* _pRuntimeReflectionData) :
            m_pRuntimeReflectionData(_pRuntimeReflectionData),
            m_pRuntimeReflection(_pReflection),
            m_heap(_heap)
        {
            m_bHasGeneratedBody = false;
        }

        ~RuntimeCompilerReflectionModule()
        {

        }

        RuntimeReflection::__data* m_pRuntimeReflectionData;
        RuntimeReflection::__ftable* m_pRuntimeReflection;
        Memory::FreeList m_heap;

        bool m_bHasGeneratedBody;
        std::vector<std::string> m_aVars;
        std::vector<std::string> m_aChildSerializers;

        void ProcessAST(Parser::__ast* _ast)
        {
            recursive(this, *_ast, &_ast->m_main);
        }

        void GenerateCustom(std::ofstream& _file)
        {
            _file << " \\\n";
            _file << "public: \\\n";
            _file << "void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \\\n";
            _file << "{\\\n";

            for(const auto& _var : m_aVars)
            {
                _file << "_pSerializer->SerializeProperty(\"" << _var << "\", " << _var << "); \\\n";
            }

            for(const std::string& _child : m_aChildSerializers)
            {
                _file << _child << "::Serialize(_pSerializer); \\\n";
            }

            _file << "}";
        }

        void Clear()
        {
            m_aVars.clear();
            m_aChildSerializers.clear();
            m_bHasGeneratedBody = false;
        }
    };

    void recursive(RuntimeCompilerReflectionModule* _pData, const Parser::__ast& _ast, Parser::__ast_entity* _entity)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            Parser::__ast_entity_structure* _struct = (Parser::__ast_entity_structure*)_entity;
            Parser::__ast_entity* _current = _struct->m_pParentScope;
            std::vector<const char*> _namespaces;

            while(_current != nullptr && _current->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
            {
                if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
                {
                    Parser::__ast_entity_namespace* _entity = (Parser::__ast_entity_namespace*)_current;

                    _namespaces.push_back(_entity->m_sName.c_str());
                }
                else if(_current->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
                {
                    Parser::__ast_entity_structure* _entity = (Parser::__ast_entity_structure*)_current;

                    _namespaces.push_back(_entity->m_sName.c_str());
                }

                _current = _current->m_pParentScope;
            }

            std::reverse(_namespaces.begin(), _namespaces.end());

            RuntimeReflection::__duckvil_resource_type_t _typeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflectionData, _struct->m_sName.c_str(), _namespaces);

            if(_typeHandle.m_ID != -1)
            {
                const Memory::Vector<RuntimeReflection::__duckvil_resource_inheritance_t>& _inhs = _pData->m_pRuntimeReflection->m_fnGetInheritances(_pData->m_pRuntimeReflectionData, _pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), _typeHandle);

                for(const auto& _inh : _inhs)
                {
                    const RuntimeReflection::__inheritance_t& _inhData = _pData->m_pRuntimeReflection->m_fnGetInheritance(_pData->m_pRuntimeReflectionData, _typeHandle, _inh);
                    RuntimeReflection::__duckvil_resource_type_t _inhTypeHandle = RuntimeReflection::get_type(_pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData, _inhData.m_ullInheritanceTypeID);

                    if(_inhTypeHandle.m_ID != -1)
                    {
                        RuntimeReflection::__duckvil_resource_function_t _funcHandle = RuntimeReflection::get_function_handle<RuntimeSerializer::ISerializer*>(_pData->m_pRuntimeReflection, _pData->m_pRuntimeReflectionData, _inhTypeHandle, "Serialize");

                        if(_funcHandle.m_ID != -1)
                        {
                            const RuntimeReflection::__type_t& _inhType = _pData->m_pRuntimeReflection->m_fnGetType(_pData->m_pRuntimeReflectionData, _inhTypeHandle);

                            _pData->m_aChildSerializers.push_back(_inhType.m_sTypeName);
                        }
                    }
                }
            }

            for(Parser::__ast_entity* _ent : _struct->m_aScopes)
            {
                if(_ent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_define)
                {
                    Parser::__ast_entity_define* _define = (Parser::__ast_entity_define*)_ent;

                    if(_define->m_sName == "DUCKVIL_GENERATED_BODY")
                    {
                        _pData->m_bHasGeneratedBody = true;
                    }
                }
            }

            if(_pData->m_bHasGeneratedBody)
            {
                Parser::__ast_entity_function* _func = new Parser::__ast_entity_function();

                _func->m_sReturnType = "void";
                _func->m_sName = "Serialize";
                _func->m_pParentScope = _struct;
                _func->m_accessLevel = Parser::__ast_access::__ast_access_public;
                _func->m_flags = (Parser::__ast_flags)0;

                {
                    Parser::__ast_entity_argument _arg;

                    _arg.m_sName = "_pSerializer";
                    _arg.m_sType = "Duckvil::RuntimeSerializer::ISerializer*";
                    _arg.m_pParentScope = _func;

                    _func->m_aArguments.push_back(_arg);
                }

                _struct->m_aScopes.push_back(_func);
            }

            for(Parser::__ast_entity* _child : _struct->m_aScopes)
            {
                if(_child->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
                {
                    Parser::__ast_entity_variable* _var = (Parser::__ast_entity_variable*)_child;
                    bool _skip = false;

                    for(const auto& _needed : _var->m_aNeededDefines)
                    {
                        bool _contain = false;

                        for(const auto& _declared : _ast.m_aUserDefines)
                        {
                            if(_needed == _declared.m_sUserDefine)
                            {
                                _contain = true;

                                break;
                            }
                        }

                        if(!_contain)
                        {
                            _skip = true;

                            break;
                        }
                    }

                    if(!_skip)
                    {
                        _pData->m_aVars.push_back(_var->m_sName);
                    }
                }
            }
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ast, _ent);
        }
    }

}}