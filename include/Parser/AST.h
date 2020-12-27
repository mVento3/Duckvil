#pragma once

#include "Utils/Macro.h"

#include "Memory/Memory.h"

#include "Parser/Lexer.h"

namespace Duckvil { namespace Parser {

    enum __ast_entity_type
    {
        __ast_entity_type_function,
        __ast_entity_type_constructor,
        __ast_entity_type_callback,
        __ast_entity_type_namespace,
        __ast_entity_type_structure,
        __ast_entity_type_variable,
        __ast_entity_type_argument,
        __ast_entity_type_enum,
        __ast_entity_type_main,
        __ast_entity_type_typedef,
        __ast_entity_type_callback_typedef,
        __ast_entity_type_define
    };

    enum __ast_structure_type
    {
        __ast_structure_type_class,
        __ast_structure_type_struct
    };

    enum __ast_access
    {
        __ast_access_public,
        __ast_access_protected,
        __ast_access_private,
        __ast_access_not_specified
    };

    enum __ast_flags : uint8_t
    {
        __ast_flags_static = 1 << 0,
        __ast_flags_const = 1 << 1,
        __ast_flags_inline = 1 << 2
    };

    struct __ast_inheritance
    {
        __ast_access m_protection;
        std::string m_sName;
    };

    struct __ast_meta
    {
        std::string m_sKey;
        std::string m_sValue;
    };

    struct __ast_template_element
    {
        std::string m_sType;
        std::string m_sName;
    };

    struct __ast_template
    {
        std::vector<__ast_template_element> m_aElements;
    };

    struct __ast_entity
    {
        __ast_entity(__ast_entity_type _scopeType = __ast_entity_type::__ast_entity_type_main) :
            m_scopeType(_scopeType)
        {

        }

        __ast_entity_type m_scopeType;
        std::vector<__ast_entity*> m_aScopes;
        __ast_entity* m_pParentScope;
        std::vector<__ast_meta> m_aMeta;
    };

    struct __ast_entity_define : public __ast_entity
    {
        __ast_entity_define(__ast_entity_type _entityType = __ast_entity_type::__ast_entity_type_define) :
            __ast_entity(_entityType)
        {

        }

        std::string m_sName;
    };

    struct __ast_entity_argument : public __ast_entity
    {
        __ast_entity_argument(__ast_entity_type _entityType = __ast_entity_type::__ast_entity_type_argument) :
            __ast_entity(_entityType)
        {

        }

        std::string m_sType;
        std::string m_sName;
    };

    struct __ast_entity_variable : public __ast_entity_argument
    {
        __ast_entity_variable(__ast_entity_type _entityType = __ast_entity_type::__ast_entity_type_variable) :
            __ast_entity_argument(_entityType)
        {

        }

        __ast_access m_accessLevel;
    };

    struct __ast_entity_structure : public __ast_entity
    {
        __ast_entity_structure(__ast_structure_type _structureType) :
            __ast_entity(__ast_entity_type::__ast_entity_type_structure),
            m_structureType(_structureType)
        {

        }

        std::string m_sName;
        __ast_structure_type m_structureType;
        std::vector<__ast_inheritance> m_aInheritance;
        std::vector<__ast_template> m_aTemplates;
    };

    struct __ast_entity_enum : public __ast_entity
    {
        __ast_entity_enum() :
            __ast_entity(__ast_entity_type::__ast_entity_type_enum)
        {

        }

        std::string m_sName;
        std::vector<std::string> m_aElements;
    };

    struct __ast_entity_namespace : public __ast_entity
    {
        __ast_entity_namespace() :
            __ast_entity(__ast_entity_type::__ast_entity_type_namespace)
        {

        }

        std::string m_sName;
    };

    struct __ast_entity_callable : public __ast_entity
    {
        __ast_entity_callable(__ast_entity_type _entityType) :
            __ast_entity(_entityType)
        {

        }

        std::vector<__ast_entity_argument> m_aArguments;
        __ast_access m_accessLevel;
    };

    struct __ast_entity_typedef : public __ast_entity
    {
        __ast_entity_typedef(__ast_entity_type _type = __ast_entity_type::__ast_entity_type_typedef) :
            __ast_entity(_type)
        {

        }

        // typedef Test<float> TestFloat;

        std::string m_sType; // like Test<float>
        std::string m_sName; // TestFloat
    };

    struct __ast_entity_callback_typedef : public __ast_entity_typedef
    {
        __ast_entity_callback_typedef() :
            __ast_entity_typedef(__ast_entity_type::__ast_entity_type_callback_typedef)
        {

        }

        // typedef float (*test)(int);

        std::vector<__ast_entity_argument> m_aArguments;
    };

    struct __ast_entity_callback : public __ast_entity_callable
    {
        __ast_entity_callback() :
            __ast_entity_callable(__ast_entity_type::__ast_entity_type_callback)
        {

        }

        std::string m_sName;
        std::string m_sReturnType;
    };

    struct __ast_entity_constructor : public __ast_entity_callable
    {
        __ast_entity_constructor() :
            __ast_entity_callable(__ast_entity_type::__ast_entity_type_constructor)
        {

        }

        std::vector<__ast_template> m_aTemplates;
    };

    struct __ast_entity_function : public __ast_entity_callable
    {
        __ast_entity_function() :
            __ast_entity_callable(__ast_entity_type::__ast_entity_type_function)
        {
            
        }

        std::string m_sName;
        std::string m_sReturnType;
        std::vector<__ast_template> m_aTemplates;
        __ast_flags m_flags;
    };

    struct __ast
    {
        __ast_entity m_main;
        __ast_entity* m_pCurrentScope;
        __ast_entity* m_pPendingScope;
        __ast_access m_currentAccess;
        std::vector<std::string> m_aUserDefines;
    };

    struct __ast_ftable
    {
        void (*ast_generate)(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData);
        void (*ast_print)(const __ast& _AST);
    };

    void ast_generate(__ast* _pAST, __lexer_ftable* _pLexer, __lexer_data& _lexerData);
    void ast_print(const __ast& _AST);

}}

DUCKVIL_EXPORT Duckvil::Parser::__ast_ftable* duckvil_ast_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);