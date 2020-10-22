#include "Parser\AST.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
using namespace Duckvil::Parser;
_type = record_type<__ast_inheritance>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_inheritance");
_property = record_property<__ast_access>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_inheritance, m_protection), "m_protection");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_inheritance, m_sName), "m_sName");
_type = record_type<__ast_meta>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_meta");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_meta, m_sKey), "m_sKey");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_meta, m_sValue), "m_sValue");
_type = record_type<__ast_template_element>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_template_element");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_template_element, m_sType), "m_sType");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_template_element, m_sName), "m_sName");
_type = record_type<__ast_template>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_template");
_property = record_property<std::vector<__ast_template_element>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_template, m_aElements), "m_aElements");
_type = record_type<__ast_entity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity");
_constructor = record_constructor<__ast_entity, __ast_entity_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<__ast_entity_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity, m_scopeType), "m_scopeType");
_property = record_property<std::vector<__ast_entity*>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity, m_aScopes), "m_aScopes");
_property = record_property<__ast_entity*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity, m_pParentScope), "m_pParentScope");
_property = record_property<std::vector<__ast_meta>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity, m_aMeta), "m_aMeta");
_type = record_type<__ast_entity_argument>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_argument");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_argument, __ast_entity_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_argument, m_sType), "m_sType");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_argument, m_sName), "m_sName");
_type = record_type<__ast_entity_variable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_variable");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity_argument>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_variable, __ast_entity_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<__ast_access>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_variable, m_accessLevel), "m_accessLevel");
_type = record_type<__ast_entity_structure>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_structure");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_structure, __ast_structure_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_structure, m_sName), "m_sName");
_property = record_property<__ast_structure_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_structure, m_structureType), "m_structureType");
_property = record_property<std::vector<__ast_inheritance>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_structure, m_aInheritance), "m_aInheritance");
_property = record_property<std::vector<__ast_template>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_structure, m_aTemplates), "m_aTemplates");
_type = record_type<__ast_entity_enum>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_enum");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_enum>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_enum, m_sName), "m_sName");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_enum, m_aElements), "m_aElements");
_type = record_type<__ast_entity_namespace>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_namespace");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_namespace>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_namespace, m_sName), "m_sName");
_type = record_type<__ast_entity_callable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_callable");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_callable, __ast_entity_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::vector<__ast_entity_argument>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_callable, m_aArguments), "m_aArguments");
_property = record_property<__ast_access>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_callable, m_accessLevel), "m_accessLevel");
_type = record_type<__ast_entity_typedef>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_typedef");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_typedef, __ast_entity_type>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_typedef, m_sType), "m_sType");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_typedef, m_sName), "m_sName");
_type = record_type<__ast_entity_callback_typedef>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_callback_typedef");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity_typedef>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_callback_typedef>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::vector<__ast_entity_argument>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_callback_typedef, m_aArguments), "m_aArguments");
_type = record_type<__ast_entity_callback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_callback");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity_callable>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_callback>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_callback, m_sName), "m_sName");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_callback, m_sReturnType), "m_sReturnType");
_type = record_type<__ast_entity_constructor>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_constructor");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity_callable>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_constructor>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::vector<__ast_template>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_constructor, m_aTemplates), "m_aTemplates");
_type = record_type<__ast_entity_function>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_entity_function");
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<__ast_entity_callable>(_pData), __protection::__protection_public);
_constructor = record_constructor<__ast_entity_function>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_function, m_sName), "m_sName");
_property = record_property<std::string>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_function, m_sReturnType), "m_sReturnType");
_property = record_property<std::vector<__ast_template>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast_entity_function, m_aTemplates), "m_aTemplates");
_type = record_type<__ast>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast");
_property = record_property<__ast_entity>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast, m_main), "m_main");
_property = record_property<__ast_entity*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast, m_pCurrentScope), "m_pCurrentScope");
_property = record_property<__ast_entity*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast, m_pPendingScope), "m_pPendingScope");
_property = record_property<__ast_access>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast, m_currentAccess), "m_currentAccess");
_property = record_property<std::vector<std::string>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(__ast, m_aUserDefines), "m_aUserDefines");
_type = record_type<__ast_ftable>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "__ast_ftable");
}
