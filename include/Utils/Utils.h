#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <type_traits>
#include <cstring>

#include "Memory/Memory.h"

namespace Duckvil { namespace Utils {

    struct string;

    void allocate(string* _pString, Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, std::size_t _ullLength);
    void allocate(string* _pString, std::size_t _ullLength);
    void allocate(string* _pString, const string& _string);

    struct string
    {
        string()
        {
            m_sText = nullptr;
            m_ullLength = 0;
            m_pMemory = nullptr;
            m_pAllocator = nullptr;
        }

        string(const string& _string, Memory::ftable* _pMemory = 0, Memory::free_list_allocator* _pAllocator = 0)
        {
            allocate(this, _pMemory, _pAllocator, _string.m_ullLength);
            memcpy(m_sText, _string.m_sText, _string.m_ullLength);
        }

        string(string&& _string) noexcept :
            m_sText(std::move(_string.m_sText)),
            m_ullLength(std::move(_string.m_ullLength)),
            m_pMemory(std::move(_string.m_pMemory)),
            m_pAllocator(std::move(_string.m_pAllocator))
        {
            _string.m_pAllocator = nullptr;
            _string.m_pMemory = nullptr;
            _string.m_sText = nullptr;
            _string.m_ullLength = 0;
        }

        string(const char* _sText, std::size_t _ullLength, Memory::ftable* _pMemory = 0, Memory::free_list_allocator* _pAllocator = 0)
        {
            allocate(this, _pMemory, _pAllocator, _ullLength);
            memcpy(m_sText, _sText, _ullLength);
        }

        template <std::size_t Length>
        string(const char (&_sText)[Length], Memory::ftable* _pMemory = 0, Memory::free_list_allocator* _pAllocator = 0) :
            string(_sText, Length, _pMemory, _pAllocator)
        {

        }

        ~string()
        {
            if(m_ullLength == 0)
            {
                return;
            }

            if(m_pMemory != nullptr && m_pAllocator != nullptr)
            {
                m_pMemory->m_fnFreeListFree_(m_pAllocator, m_sText);
            }
            else
            {
                delete[] m_sText;
            }
        }

        char* m_sText;
        std::size_t m_ullLength;
        Memory::ftable* m_pMemory = 0;
        Memory::free_list_allocator* m_pAllocator = 0;

        template <std::size_t Length>
        string& operator=(const char (&_sText)[Length])
        {
            allocate(this, Length);
            memcpy(m_sText, _sText, Length);

            return *this;
        }

        string& operator=(string&& _string) noexcept
        {
            if(&_string == this)
            {
                return *this;
            }

            m_sText = std::move(_string.m_sText);
            m_ullLength = std::move(_string.m_ullLength);
            m_pMemory = std::move(_string.m_pMemory);
            m_pAllocator = std::move(_string.m_pAllocator);

            _string.m_sText = nullptr;
            _string.m_ullLength = 0;
            _string.m_pMemory = nullptr;
            _string.m_pAllocator = nullptr;

            return *this;
        }

        string& operator=(const string& _string)
        {
            m_ullLength = _string.m_ullLength;

            allocate(this, _string);
            memcpy(m_sText, _string.m_sText, m_ullLength);

            return *this;
        }
    };

    static inline bool operator==(const string& _lhs, const string& _rhs);

    template <size_t Length>
    static inline bool operator==(const string& _lhs, const char (&_rhs)[Length])
    {
        if(_lhs.m_ullLength == -1 || Length == -1)
        {
            return false;
        }

        return strcmp(_lhs.m_sText, _rhs) == 0;
    }

    // static inline bool operator==(const string& _lhs, const char* _rhs)
    // {
    //     if(_lhs.m_ullLength == -1)
    //     {
    //         return false;
    //     }

    //     return strcmp(_lhs.m_sText, _rhs) == 0;
    // }

    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
    std::vector<std::string> split(const std::string& s, char delim);

    std::size_t find(const char* _sText, char _cToken, std::size_t _ullLength);

    template <std::size_t Length>
    void join_string(string& _sBuffer, const char (&_sText)[Length], std::size_t& _ullLength)
    {
        memcpy(_sBuffer.m_sText + _ullLength, _sText, Length);
        _ullLength += Length - 1;
    }

    template <std::size_t Length>
    void calculate_string_length(const char (&_sText)[Length], std::size_t& _ullLength)
    {
        _ullLength += Length - 1;
    }

    void join_string(string& _sBuffer, const string& _sText, std::size_t& _ullLength);
    void calculate_string_length(const string& _sText, std::size_t& _ullLength);

    template <typename... A>
    void join(string& _buffer, A&& ... _sText)
    {
        if(_buffer.m_ullLength == 0)
        {
            std::size_t _ullLength = 0;
            int _[] = { 0, (calculate_string_length(_sText, _ullLength), 0)... };

            // _buffer.Allocate(_ullLength + 1);
            allocate(&_buffer, _ullLength + 1);
        }

        std::size_t _index = 0;
        int _[] = { 0, (join_string(_buffer, _sText, _index), 0)... };
    }

}}