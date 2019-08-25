CMAKE_MINIMUM_REQUIRED (VERSION 3.5.0 FATAL_ERROR)

PROJECT (buchberger VERSION 0.2.3 LANGUAGES CXX)


IF (NOT CMAKE_BUILD_TYPE)
    SET (CMAKE_BUILD_TYPE RELEASE)
ENDIF ()

MESSAGE ("-- Build type: ${CMAKE_BUILD_TYPE}")

IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    ADD_COMPILE_OPTIONS (-std=c++14)

    ADD_COMPILE_OPTIONS (-Wall)
    ADD_COMPILE_OPTIONS (-Wcast-align)
    ADD_COMPILE_OPTIONS (-Wcast-qual)
    ADD_COMPILE_OPTIONS (-Wconversion)
    ADD_COMPILE_OPTIONS (-Wctor-dtor-privacy)
    ADD_COMPILE_OPTIONS (-Wenum-compare)
    ADD_COMPILE_OPTIONS (-Werror)
    ADD_COMPILE_OPTIONS (-Wextra)
    ADD_COMPILE_OPTIONS (-Wfloat-equal)
    ADD_COMPILE_OPTIONS (-Wnon-virtual-dtor)
    ADD_COMPILE_OPTIONS (-Wold-style-cast)
    ADD_COMPILE_OPTIONS (-Woverloaded-virtual)
    ADD_COMPILE_OPTIONS (-Wpedantic)
    ADD_COMPILE_OPTIONS (-Wredundant-decls)
    ADD_COMPILE_OPTIONS (-Wsign-conversion)
    ADD_COMPILE_OPTIONS (-Wsign-promo)
    ADD_COMPILE_OPTIONS (-Wshadow)
    ADD_COMPILE_OPTIONS (-Wunused)

    SET (CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops")
    SET (CMAKE_CXX_FLAGS_DEBUG   "-O0 -ggdb -g3")

ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    ADD_COMPILE_OPTIONS (/std:c++14)

    ADD_COMPILE_OPTIONS (/W4)
    ADD_COMPILE_OPTIONS (/WX)
    ADD_COMPILE_OPTIONS (/MP)

    SET (CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2")
    SET (CMAKE_CXX_FLAGS_DEBUG   "/Od /Zi")

ENDIF ()


CONFIGURE_FILE (
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/current_version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/current_version.h"
)

SET (HEADERS
    "src/allocator.h"
    "src/groebner.h"
    "src/init.h"
    "src/monom.h"
    "src/polynom.h"
    "src/timer.h"
    "src/util.h"
    "src/variables.h"
    "src/version.h"
)

SET (SOURCES
    "src/allocator.cpp"
    "src/groebner.cpp"
    "src/init.cpp"
    "src/main.cpp"
    "src/monom.cpp"
    "src/polynom.cpp"
    "src/timer.cpp"
    "src/variables.cpp"
    "src/version.cpp"
)

IF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    CONFIGURE_FILE (
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/buchberger.rc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/buchberger.rc"
    )

    SET (RESOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/buchberger.rc"
    )
ENDIF ()


ADD_EXECUTABLE (${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES}
)

TARGET_INCLUDE_DIRECTORIES (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
