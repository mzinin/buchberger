#pragma once

#include <cassert>
#include <cstdio>
#include <cstdlib>


#ifdef NDEBUG
    #define IASSERT(a)
    #define IASSERT2(file, line, a)
    #define IASSERTVALID(a)
    #define IASSERT_VALID(a)
#else
    #define IASSERT(a)              assert(a)
    #define IASSERT2(file, line, a) if (!(a)) { printf("from file: '%s'  line: %d\n", file, line); assert(a); }
    #define IASSERTVALID(a)         (a).assertValid(__FILE__, __LINE__)
    #define IASSERT_VALID(a)        assertValid(a,__FILE__, __LINE__)
#endif // NDEBUG


#define IERROR(a)     printf("**** file: %s\n     line: %d\n    error: %s\n", __FILE__, __LINE__, a), abort()
#define IMESSAGE(a)   printf("**** file: %s\n     line: %d\n  message: %s\n", __FILE__, __LINE__, a)
