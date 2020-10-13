#pragma once

#include "Utils/Macro.h"

#include <vector>
#include <string>

#include "Memory/Memory.h"

#define DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH 256

namespace Duckvil { namespace Parser {

    enum __lexer_token_type
    {
        __lexer_token_type_symbol, // (, ), &, !, etc.
        __lexer_token_type_number, // 1, 2, 12, 1423, 1.2, etc.
        __lexer_token_type_text, // test, func, etc.
        __lexer_token_type_string, // "aaa", "bbb", etc.
        __lexer_token_type_floating,
        __lexer_token_type_integer,
        __lexer_token_type_negative_integer,
        __lexer_token_type_negative_floating
    };

    struct __lexer_token
    {
        __lexer_token_type m_tokenType;
    };

    struct __lexer_data
    {
        std::vector<std::string> m_aLines;
        std::string m_sCurrentLine;
        uint32_t m_uiCurrentLine;
        uint32_t m_uiCurrentCharacterIndex;
        std::string m_sToken;
        char m_cCurrentCharacter;
        bool m_bWasSpace;
        bool m_bSpace;
        bool m_bNewLine;
        bool m_bEnd;
    };

    struct __lexer_ftable
    {
        void (*load_file)(__lexer_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
        bool (*next_token)(__lexer_data* _pData, std::string* _pToken);
        bool (*space)(const __lexer_data& _data);
    };

    void load_file(__lexer_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
    bool next_token(__lexer_data* _pData, std::string* _pToken);
    bool space(const __lexer_data& _data);

}}

DUCKVIL_EXPORT Duckvil::Parser::__lexer_ftable* duckvil_lexer_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);