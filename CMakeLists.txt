CMAKE_MINIMUM_REQUIRED (VERSION 3.5.0 FATAL_ERROR)

PROJECT (buhberger VERSION 0.2.3 LANGUAGES CXX)


IF (NOT CMAKE_BUILD_TYPE)
    SET (CMAKE_BUILD_TYPE RELEASE)
ENDIF ()

MESSAGE ("-- Build type: ${CMAKE_BUILD_TYPE}")

IF (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    ADD_COMPILE_OPTIONS (-std=c++14)

    SET (CMAKE_CXX_FLAGS_RELEASE "-O3 -funroll-loops")
    SET (CMAKE_CXX_FLAGS_DEBUG   "-O0 -ggdb -g3")

ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    ADD_COMPILE_OPTIONS (/std:c++03)

    SET (CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2")
    SET (CMAKE_CXX_FLAGS_DEBUG   "/Od /Zi")

ENDIF ()


CONFIGURE_FILE (
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/current_version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/current_version.h"
)

SET (HEADERS
    "src/iallocator.h"
    "src/igroebner64.h"
    "src/init64.h"
    "src/iutil.h"
    "src/ivariables.h"
    "src/monom64.h"
    "src/poly64.h"
    "src/timer.h"
    "src/version.h"
)

SET (SOURCES
    "src/iallocator.cpp"
    "src/igroebner64.cpp"
    "src/ivariables.cpp"
    "src/main.cpp"
    "src/monom64.cpp"
    "src/poly64.cpp"
    "src/timer.cpp"
    "src/version.cpp"
)

IF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    CONFIGURE_FILE (
        "${CMAKE_CURRENT_SOURCE_DIR}/resources/buhberger.rc.in"
        "${CMAKE_CURRENT_BINARY_DIR}/buhberger.rc"
    )

    SET (RESOURCES
        "${CMAKE_CURRENT_BINARY_DIR}/buhberger.rc"
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
