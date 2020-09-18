#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <type_traits>
#include <cstring>

namespace Duckvil { namespace Utils {

    struct string
    {
        string()
        {
            m_sText = 0;
            m_ullLength = 0;
        }

        string(const string& _string)
        {
            Allocate(_string.m_ullLength);
            memcpy(m_sText, _string.m_sText, _string.m_ullLength);
        }

        string(std::size_t _ullLength)
        {
            Allocate(_ullLength);
        }

        template <std::size_t Size>
        string(const char (&_sText)[Size])
        {
            Allocate(Size);
            memcpy(m_sText, _sText, Size);
        }

        ~string()
        {
            delete[] m_sText;
        }

        void Allocate(std::size_t _ullLength)
        {
            m_sText = new char[_ullLength];
            m_ullLength = _ullLength;

            memset(m_sText, 0, _ullLength);
        }

        char* m_sText;
        std::size_t m_ullLength;
    };

    void split(const std::string& s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
    std::vector<std::string> split(const std::string& s, char delim);

    template <std::size_t Size>
    void join_string(string& _sBuffer, const char (&_sText)[Size], std::size_t& _ullLength)
    {
        memcpy(_sBuffer.m_sText + _ullLength, _sText, Size);
        _ullLength += Size - 1;
    }

    template <std::size_t Size>
    void calculate_string_length(const char (&_sText)[Size], std::size_t& _ullLength)
    {
        _ullLength += Size - 1;
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

            _buffer.Allocate(_ullLength + 1);
        }

        std::size_t _index = 0;
        int _[] = { 0, (join_string(_buffer, _sText, _index), 0)... };
    }

}}