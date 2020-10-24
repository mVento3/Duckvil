#include "RuntimeReflection/Generator.h"

#include <fstream>

namespace Duckvil { namespace RuntimeReflection {

    std::string combine_namespace(std::queue<__generator_namespace> _namespaces)
    {
        std::string _res;

        while(!_namespaces.empty())
        {
            _res += std::string(_namespaces.front().m_sName);

            if(_namespaces.size() > 1)
            {
                _res += "::";
            }

            _namespaces.pop();
        }

        return _res;
    }

    void recursive(__generator_data* _pData, const Parser::__ast_entity* _entity, std::ofstream& _file)
    {
        if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
        {
            const Parser::__ast_entity_namespace* _casted = (const Parser::__ast_entity_namespace*)_entity;
            __generator_namespace _namespace = {};

            strcpy(_namespace.m_sName, _casted->m_sName.c_str());

            for(Parser::__ast_meta _meta : _entity->m_aMeta)
            {
                _namespace.m_aMeta.push_back(_meta);
            }

            _pData->m_aNamespaces.push(_namespace);
        }
        else if(_entity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
        {
            const Parser::__ast_entity_structure* _casted = (const Parser::__ast_entity_structure*)_entity;
            const Parser::__ast_entity_structure* _parent = _casted;
            std::string _additionalNamespace;

            if(_casted->m_aTemplates.size() == 0)
            {
                while(_parent->m_pParentScope->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_structure)
                {
                    _additionalNamespace += ((Parser::__ast_entity_structure*)_parent->m_pParentScope)->m_sName + "::";

                    _parent = (Parser::__ast_entity_structure*)_parent->m_pParentScope;
                }

                if(!_pData->m_bWasNamespaces)
                {
                    _pData->m_bWasNamespaces = true;

                    std::string _combined = combine_namespace(_pData->m_aNamespaces);

                    if(_combined.size() > 0)
                    {
                        _file << "using namespace " + _combined + ";\n";
                    }
                }

                _file << "_type = record_type<" + _additionalNamespace + _casted->m_sName + ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, \"" + _casted->m_sName + "\");\n";

                Parser::__ast_entity* _namespace = _casted->m_pParentScope;
                std::stack<Parser::__ast_entity_namespace*> _namespaces;

                while(_namespace->m_scopeType != Parser::__ast_entity_type::__ast_entity_type_main)
                {
                    if(_namespace->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_namespace)
                    {
                        _namespaces.push((Parser::__ast_entity_namespace*)_namespace);
                    }

                    _namespace = _namespace->m_pParentScope;
                }

                while(!_namespaces.empty())
                {
                    Parser::__ast_entity_namespace* _n = _namespaces.top();

                    _file << "record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, \"" << _n->m_sName << "\");\n";

                    _namespaces.pop();
                }

                for(const Parser::__ast_meta& _meta : _casted->m_aMeta)
                {
                    _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
                }

                for(const Parser::__ast_inheritance& _inheritance : _casted->m_aInheritance)
                {
                    _file << "record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<" + _inheritance.m_sName + ">(_pData), ";

                    if(_inheritance.m_protection == Parser::__ast_access::__ast_access_public)
                    {
                        _file << "__protection::__protection_public);\n";
                    }
                    else if(_inheritance.m_protection == Parser::__ast_access::__ast_access_protected)
                    {
                        _file << "__protection::__protection_protected);\n";
                    }
                    else if(_inheritance.m_protection == Parser::__ast_access::__ast_access_private)
                    {
                        _file << "__protection::__protection_private);\n";
                    }
                }

                for(Parser::__ast_entity* _ent : _entity->m_aScopes)
                {
                    if(_ent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_variable)
                    {
                        const Parser::__ast_entity_variable* _castedVariable = (const Parser::__ast_entity_variable*)_ent;

                        if(_castedVariable->m_accessLevel == Parser::__ast_access::__ast_access_public)
                        {
                            std::string _additionalNamespaceTypedef;

                            for(Parser::__ast_entity* _typedefEntity : _casted->m_aScopes)
                            {
                                if(_typedefEntity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_callback_typedef)
                                {
                                    Parser::__ast_entity_callback_typedef* _castedTypedefEntity = (Parser::__ast_entity_callback_typedef*)_typedefEntity;

                                    if(_castedTypedefEntity->m_sName == _castedVariable->m_sType)
                                    {
                                        _additionalNamespaceTypedef += ((Parser::__ast_entity_structure*)_casted)->m_sName + "::";

                                        break;
                                    }
                                }
                                else if(_typedefEntity->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_typedef)
                                {
                                    Parser::__ast_entity_typedef* _castedTypedefEntity = (Parser::__ast_entity_typedef*)_typedefEntity;

                                    if(_castedTypedefEntity->m_sName == _castedVariable->m_sType)
                                    {
                                        _additionalNamespaceTypedef += ((Parser::__ast_entity_structure*)_casted)->m_sName + "::";

                                        break;
                                    }
                                }
                            }

                            _file << "_property = record_property<" + _additionalNamespaceTypedef + _castedVariable->m_sType + ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(" + _additionalNamespace + _casted->m_sName + ", " + _castedVariable->m_sName + "), \"" + _castedVariable->m_sName + "\");\n";

                            for(const Parser::__ast_meta& _meta : _castedVariable->m_aMeta)
                            {
                                _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _property, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
                            }
                        }
                    }
                    else if(_ent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_constructor)
                    {
                        // TODO: Not always generate, idk why
                        const Parser::__ast_entity_constructor* _castedConstructor = (const Parser::__ast_entity_constructor*)_ent;

                        if(_castedConstructor->m_accessLevel == Parser::__ast_access::__ast_access_public)
                        {
                            _file << "_constructor = record_constructor<" + _additionalNamespace + _casted->m_sName;

                            if(!_castedConstructor->m_aArguments.empty())
                            {
                                _file << ", ";

                                for(uint32_t i = 0; i < _castedConstructor->m_aArguments.size(); i++)
                                {
                                    _file << _castedConstructor->m_aArguments[i].m_sType;

                                    if(i < _castedConstructor->m_aArguments.size() - 1)
                                    {
                                        _file << ", ";
                                    }
                                }
                            }

                            _file << ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);\n";

                            for(const Parser::__ast_meta& _meta : _castedConstructor->m_aMeta)
                            {
                                _file << "record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _constructor, " + _meta.m_sKey + ", " + _meta.m_sValue + ");\n";
                            }
                        }
                    }
                    else if(_ent->m_scopeType == Parser::__ast_entity_type::__ast_entity_type_function)
                    {
                        const Parser::__ast_entity_function* _castedFunction = (const Parser::__ast_entity_function*)_ent;

                        if(_castedFunction->m_accessLevel == Parser::__ast_access::__ast_access_public)
                        {
                            _file << "record_function<";

                            if(_castedFunction->m_flags & Parser::__ast_flags::__ast_flags_static)
                            {
                                _file << _castedFunction->m_sReturnType;
                            }
                            else
                            {
                                _file << _additionalNamespace + _casted->m_sName + ", ";
                                _file << _castedFunction->m_sReturnType;
                            }

                            if(!_castedFunction->m_aArguments.empty())
                            {
                                _file << ", ";

                                for(uint32_t i = 0; i < _castedFunction->m_aArguments.size(); i++)
                                {
                                    _file << _castedFunction->m_aArguments[i].m_sType;

                                    if(i < _castedFunction->m_aArguments.size() - 1)
                                    {
                                        _file << ", ";
                                    }
                                }
                            }

                            _file << ">(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &" << _additionalNamespace + _casted->m_sName + "::" + _castedFunction->m_sName << ", \"" << _castedFunction->m_sName << "\");\n";
                        }
                    }
                }
            }

            // _pData->m_pCurrent = _casted;
        }

        for(Parser::__ast_entity* _ent : _entity->m_aScopes)
        {
            recursive(_pData, _ent, _file);
        }
    }

    __generator_data* init(Memory::IMemory* _pMemory, Memory::__free_list_allocator* _pAllocator)
    {
        __generator_data* _data = (__generator_data*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__generator_data), alignof(__generator_data));

        // _data->m_aNamespaces = Memory::Vector<const char*>(_pMemory, _pAllocator, 3);

        _data->m_bWasNamespaces = false;

        return _data;
    }

    void generate(__generator_data* _pData, const char _sPath[DUCKVIL_RUNTIME_REFLECTION_GENERATOR_PATH_LENGTH_MAX], const Parser::__ast& _ast)
    {
        std::ofstream _file(_sPath);

        _file << "#include \"" << _pData->m_sInclude << "\"\n";
        _file << "#include \"RuntimeReflection/Recorder.h\"\n";
        // TODO: Include generated .h file

        _file << "DUCKVIL_RUNTIME_REFLECTION_RECORD(" << _pData->m_uiRecorderIndex << ")\n";
        _file << "{\n";

        _file << "using namespace Duckvil::RuntimeReflection;\n";
        _file << "using namespace Duckvil;\n";
        _file << "DUCKVIL_RESOURCE(type_t) _type;\n";
        _file << "DUCKVIL_RESOURCE(property_t) _property;\n";
        _file << "DUCKVIL_RESOURCE(constructor_t) _constructor;\n";

        recursive(_pData, &_ast.m_main, _file);

        _file << "}\n";

        _file.close();
    }

}}

Duckvil::RuntimeReflection::__generator_ftable* duckvil_runtime_reflection_generator_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::RuntimeReflection::__generator_ftable* _ftable = (Duckvil::RuntimeReflection::__generator_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::RuntimeReflection::__generator_ftable), alignof(Duckvil::RuntimeReflection::__generator_ftable));

    _ftable->init = &Duckvil::RuntimeReflection::init;
    _ftable->generate = &Duckvil::RuntimeReflection::generate;

    return _ftable;
}