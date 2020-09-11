#pragma once

#include "Utils/Macro.h"

#include <cstdint>
#include <cstdio>
#include <stdexcept>

namespace Duckvil { namespace UniTestFramework {

    static bool g_bTestsFailed = false;

    struct Assert
    {
        template <typename A>
        static bool AreEqual(const A& v1, const A& v2)
        {
            return v1 == v2;
        }

        template <typename A>
        static bool AreEqual(const A* v1, const A& v2)
        {
            if(v1 == NULL)
            {
                return false;
            }

            return *v1 == v2;
        }

        template <typename A>
        static bool NotEqual(const A& v1, const A& v2)
        {
            return v1 != v2;
        }

        template <typename A>
        static bool NotEqual(const A* v1, const A& v2)
        {
            if (v1 == NULL)
            {
                return false;
            }

            return *v1 != v2;
        }

        template <typename A>
        static bool IsNULL(const A* v1)
        {
            return v1 == 0;
        }

        template <typename A>
        static bool IsNotNULL(const A* v1)
        {
            return v1 != 0;
        }
    };

    static void test_failed(const char* _sTestName, const char* _sReason, const char* _sFile = "", uint32_t _uiLine = 0)
    {
        printf("TEST(%s:%u) '%s' failed! Reason: %s\n", _sFile, _uiLine, _sTestName, _sReason);

        if(!g_bTestsFailed)
        {
            g_bTestsFailed = true;
        }

        // throw std::runtime_error("AAA");
    }

}}

#define DUCKVIL_TEST(name) \
namespace DUCKVIL_CAT(duckvil_unitest_, name) { \
static const char* test_failed_message = ""; \
static uint32_t test_failed_line = 0; \
static bool unit_test(); \
struct __test \
{ \
__test() \
{ \
if(!unit_test()) Duckvil::UniTestFramework::test_failed(#name, test_failed_message, __FILE__, test_failed_line); \
} \
}; \
static const __test g_test; \
} \
bool DUCKVIL_CAT(duckvil_unitest_, name)::unit_test()

#define DUCKVIL_TEST_EQUAL(a, b, msg) do { if(!Duckvil::UniTestFramework::Assert::AreEqual(a, b)) { test_failed_message = msg; test_failed_line = __LINE__; return false; } } while(0)
#define DUCKVIL_TEST_NOT_EQUAL(a, b, msg) do { if(!Duckvil::UniTestFramework::Assert::NotEqual(a, b)) { test_failed_message = msg; test_failed_line = __LINE__; return false; } } while(0)
#define DUCKVIL_TEST_IS_NULL(a, msg) do { if(!Duckvil::UniTestFramework::Assert::IsNULL(a)) { test_failed_message = msg; test_failed_line = __LINE__; return false; } } while(0)
#define DUCKVIL_TEST_IS_NOT_NULL(a, msg) do { if(!Duckvil::UniTestFramework::Assert::IsNotNULL(a)) { test_failed_message = msg; test_failed_line = __LINE__; return false; } } while(0)
#define DUCKVIL_TEST_EXP(exp, msg) do { if(!(exp)) { test_failed_message = msg; test_failed_line = __LINE__; return false; } } while(0)
#define DUCKVIL_TEST_SUCCESS_PASS return true
#define DUCKVIL_TEST_FAILED Duckvil::UniTestFramework::g_bTestsFailed