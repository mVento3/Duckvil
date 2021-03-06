#include "RuntimeReflection/Recorder.h"

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace RuntimeReflection {

    DUCKVIL_RESOURCE(type_t) record_type(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, __data* _pData, std::size_t _ullTypeID, const char* _sTypeName, std::size_t _ullLength)
    {
        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_pData->m_aTypes.m_data); ++i)
        {
            __type_t _type = DUCKVIL_SLOT_ARRAY_GET(_pData->m_aTypes, i);

            if(_type.m_ullTypeID == _ullTypeID)
            {
                FrameMarkStart("CleaningOldTypeReflection");

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_functions.m_data); ++j)
                {
                    __function_t& _func = DUCKVIL_SLOT_ARRAY_GET(_type.m_functions, j);

                    Memory::free_list_free(_pMemoryInterface, _pAllocator, _func.m_pFunction);
                    Memory::free_list_free(_pMemoryInterface, _pAllocator, _func.m_sFunctionName);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantKeys.m_data); ++j)
                {
                    __variant_t& _variant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantKeys, j);

                    Memory::free_list_free(_pMemoryInterface, _pAllocator, _variant.m_variant.m_pData);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_variantValues.m_data); ++j)
                {
                    __variant_t& _variant = DUCKVIL_SLOT_ARRAY_GET(_type.m_variantValues, j);

                    Memory::free_list_free(_pMemoryInterface, _pAllocator, _variant.m_variant.m_pData);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_constructors.m_data); ++j)
                {
                    __constructor_t& _constructor = DUCKVIL_SLOT_ARRAY_GET(_type.m_constructors, j);

                    DUCKVIL_SLOT_ARRAY_FREE(_pMemoryInterface, _pAllocator, _constructor.m_arguments);
                    DUCKVIL_SLOT_ARRAY_FREE(_pMemoryInterface, _pAllocator, _constructor.m_metas);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_properties.m_data); ++j)
                {
                    __property_t& _property = DUCKVIL_SLOT_ARRAY_GET(_type.m_properties, j);

                    DUCKVIL_SLOT_ARRAY_FREE(_pMemoryInterface, _pAllocator, _property.m_metas);
                }

                for(uint32_t j = 0; j < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type.m_namespaces.m_data); ++j)
                {
                    __namespace_t& _namespace = DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, j);

                    Memory::free_list_free(_pMemoryInterface, _pAllocator, _namespace.m_sNamespaceName);
                }

                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_constructors);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_properties);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_namespaces);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_inheritances);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_functions);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_variantKeys);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_variantValues);
                DUCKVIL_SLOT_ARRAY_CLEAR(_type.m_metas);

                FrameMarkEnd("CleaningOldTypeReflection");

                return _type.m_uiSlotIndex;
            }
        }

        __type_t _type = {};

        _type.m_ullTypeID =     _ullTypeID;
        _type.m_constructors =  DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __constructor_t);
        _type.m_properties =    DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __property_t);
        _type.m_namespaces =    DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __namespace_t);
        _type.m_inheritances =  DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __inheritance_t);
        _type.m_functions =     DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __function_t);
        _type.m_variantKeys =   DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __variant_t);
        _type.m_variantValues = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __variant_t);
        _type.m_metas =         DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __meta_t);

        // _type.m_sTypeName = new char[_ullLength];
        _type.m_sTypeName = (char*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _ullLength, 8);

        memcpy(_type.m_sTypeName, _sTypeName, _ullLength);

        uint32_t _type_handle = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _pData->m_aTypes, _type);

        DUCKVIL_RESOURCE(type_t) _handle = {};

        _handle.m_ID = _type_handle;

        DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _type_handle)->m_uiSlotIndex = _handle;

        return _handle;
    }

    DUCKVIL_RESOURCE(meta_t) record_type_meta(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type->m_variantKeys.m_data); ++i)
        {
            __variant_t _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, i);

            if(_key.m_owner == __variant_owner::__variant_owner_type &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
                _keyFound = true;

                break;
            }
        }

        if(!_keyFound)
        {
            DUCKVIL_RESOURCE(variant_t) _keyHandle = {};
            DUCKVIL_RESOURCE(variant_t) _valueHandle = {};

            {
                __variant_t _keyVariant = {};

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_type;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_type;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(meta_t) record_property_meta(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(property_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _typeHandle.m_ID);
        __property_t* _property = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_properties, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type->m_variantKeys.m_data); ++i)
        {
            __variant_t _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, i);

            if(_key.m_owner == __variant_owner::__variant_owner_property &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
                _keyFound = true;

                break;
            }
        }

        if(!_keyFound)
        {
            DUCKVIL_RESOURCE(variant_t) _keyHandle = {};
            DUCKVIL_RESOURCE(variant_t) _valueHandle = {};

            {
                __variant_t _keyVariant = {};

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_property;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_property;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _property->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(meta_t) record_constructor_meta(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _typeHandle, DUCKVIL_RESOURCE(constructor_t) _owner, const __recorder_meta_info& _meta)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _typeHandle.m_ID);
        __constructor_t* _property = DUCKVIL_SLOT_ARRAY_GET_POINTER(_type->m_constructors, _owner.m_ID);
        bool _keyFound = false;

        for(uint32_t i = 0; i < DUCKVIL_DYNAMIC_ARRAY_SIZE(_type->m_variantKeys.m_data); ++i)
        {
            __variant_t _key = DUCKVIL_SLOT_ARRAY_GET(_type->m_variantKeys, i);

            if(_key.m_owner == __variant_owner::__variant_owner_constructor &&
                _key.m_variant.m_ullTypeID == _meta.m_ullKeyTypeID &&
                _key.m_variant.m_ullSize == _meta.m_ullKeyTypeSize &&
                memcmp(_key.m_variant.m_pData, _meta.m_pKeyData, _meta.m_ullKeyTypeSize) == 0)
            {
                _keyFound = true;

                break;
            }
        }

        if(!_keyFound)
        {
            DUCKVIL_RESOURCE(variant_t) _keyHandle = {};
            DUCKVIL_RESOURCE(variant_t) _valueHandle = {};

            {
                __variant_t _keyVariant = {};

                _keyVariant.m_variant.m_ullTypeID = _meta.m_ullKeyTypeID;
                _keyVariant.m_variant.m_ullSize = _meta.m_ullKeyTypeSize;
                _keyVariant.m_variant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _keyVariant.m_variant.m_ullSize, _meta.m_ucKeyTypeAlignment);
                _keyVariant.m_variant.m_traits = _meta.m_keyTraits;
                _keyVariant.m_owner = __variant_owner::__variant_owner_constructor;

                memcpy(_keyVariant.m_variant.m_pData, _meta.m_pKeyData, _keyVariant.m_variant.m_ullSize);

                _keyHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_variantKeys, _keyVariant);
            }

            {
                __variant_t _valueVariant = {};

                _valueVariant.m_variant.m_ullTypeID = _meta.m_ullValueTypeID;
                _valueVariant.m_variant.m_ullSize = _meta.m_ullValueTypeSize;
                _valueVariant.m_variant.m_pData = _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _valueVariant.m_variant.m_ullSize, _meta.m_ucValueTypeAlignment);
                _valueVariant.m_variant.m_traits = _meta.m_valueTraits;
                _valueVariant.m_owner = __variant_owner::__variant_owner_constructor;

                memcpy(_valueVariant.m_variant.m_pData, _meta.m_pValueData, _valueVariant.m_variant.m_ullSize);

                _valueHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_variantValues, _valueVariant);
            }

            if(_keyHandle.m_ID != _valueHandle.m_ID)
            {
                // Something gone wrong... should not happen

                return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
            }

            __meta_t _metaData = {};

            _metaData.m_key = _keyHandle;
            _metaData.m_value = _valueHandle;
            _metaData.m_uiOwner = _owner.m_ID;

            DUCKVIL_RESOURCE(meta_t) _metaHandle = {};

            _metaHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _property->m_metas, _metaData);

            return _metaHandle;
        }

        return { DUCKVIL_SLOT_ARRAY_INVALID_HANDLE };
    }

    DUCKVIL_RESOURCE(constructor_t) record_constructor(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, uint8_t* _pConctructor, Memory::Queue<__argument_t>& _arguments)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __constructor_t _constructor = {};

        _constructor.m_ullTypeID = _ullTypeID;
        _constructor.m_owner = _owner;
        _constructor.m_pData = _pConctructor;
        _constructor.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __meta_t);
        _constructor.m_arguments = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __argument_t);

        while(!_arguments.Empty())
        {
            auto& _argument = _arguments.Begin();

            DUCKVIL_RESOURCE(constructor_t) _argumentHandle = {};

            _argumentHandle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _constructor.m_arguments, _argument);

            _arguments.Pop();
        }

        DUCKVIL_RESOURCE(constructor_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_constructors, _constructor);

        return _handle;
    }

    DUCKVIL_RESOURCE(property_t) record_property(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullTypeID, const char* _sName, std::size_t _ullLength, uintptr_t _ullAddress)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __property_t _property = {};

        _property.m_ullAddress = _ullAddress;
        _property.m_ullTypeID = _ullTypeID;
        _property.m_owner = _owner;
        _property.m_metas = DUCKVIL_SLOT_ARRAY_NEW(_pMemoryInterface, _pAllocator, __meta_t);

        // _property.m_sName = new char[_ullLength];
        _property.m_sName = (char*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _ullLength, 8);

        memcpy(_property.m_sName, _sName, _ullLength);

        DUCKVIL_RESOURCE(property_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_properties, _property);

        return _handle;
    }

    DUCKVIL_RESOURCE(namespace_t) record_namespace(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, const char* _sName, std::size_t _ullLength)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __namespace_t _namespace = {};

        // _namespace.m_sNamespaceName = new char[_ullLength];
        _namespace.m_sNamespaceName = (char*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _ullLength, 8);

        memcpy(_namespace.m_sNamespaceName, _sName, _ullLength);

        DUCKVIL_RESOURCE(namespace_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_namespaces, _namespace);

        return _handle;
    }

    DUCKVIL_RESOURCE(inheritance_t) record_inheritance(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, std::size_t _ullInheritanceTypeID, __protection _protection)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __inheritance_t _inheritance = {};

        _inheritance.m_ullInheritanceTypeID = _ullInheritanceTypeID;
        _inheritance.m_protection = _protection;

        DUCKVIL_RESOURCE(inheritance_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_inheritances, _inheritance);

        return _handle;
    }

    DUCKVIL_RESOURCE(function_t) record_function(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, __data* _pData, DUCKVIL_RESOURCE(type_t) _owner, __ifunction* _pFunction, const char* _sName, std::size_t _ullLength, std::size_t _ullReturnTypeID, std::size_t _ullArgumentsTypeID)
    {
        __type_t* _type = DUCKVIL_SLOT_ARRAY_GET_POINTER(_pData->m_aTypes, _owner.m_ID);
        __function_t _function = {};

        _function.m_pFunction = _pFunction;
        _function.m_ullReturnTypeID = _ullReturnTypeID;
        _function.m_ullArgumentsTypeID = _ullArgumentsTypeID;

        // _function.m_sFunctionName = new char[_ullLength];
        _function.m_sFunctionName = (char*)_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, _ullLength, 8);

        memcpy(_function.m_sFunctionName, _sName, _ullLength);

        DUCKVIL_RESOURCE(function_t) _handle = {};

        _handle.m_ID = duckvil_slot_array_insert(_pMemoryInterface, _pAllocator, _type->m_functions, _function);

        return _handle;
    }

}}

Duckvil::RuntimeReflection::__recorder_ftable* duckvil_runtime_reflection_recorder_init()
{
    static Duckvil::RuntimeReflection::__recorder_ftable _functions = { 0 };

    _functions.m_fnRecordType = &Duckvil::RuntimeReflection::record_type;
    _functions.m_fnRecordConstructor = &Duckvil::RuntimeReflection::record_constructor;
    _functions.m_fnRecordProperty = &Duckvil::RuntimeReflection::record_property;
    _functions.m_fnRecordNamespace = &Duckvil::RuntimeReflection::record_namespace;
    _functions.m_fnRecordInheritance = &Duckvil::RuntimeReflection::record_inheritance;
    _functions.m_fnRecordFunction = &Duckvil::RuntimeReflection::record_function;
    _functions.m_fnRecordTypeMeta = &Duckvil::RuntimeReflection::record_type_meta;
    _functions.m_fnRecordPropertyMeta = &Duckvil::RuntimeReflection::record_property_meta;
    _functions.m_fnRecordConstructorMeta = &Duckvil::RuntimeReflection::record_constructor_meta;

    return &_functions;
}