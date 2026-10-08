cmake_minimum_required(VERSION 3.15)
project(kronkpool LANGUAGES C)

include(cmake/ProjectOptions.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/Utils.cmake)

option(KRONKPOOL_BUILD_TESTS "Build kronkpool tests" ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

file(GLOB_RECURSE KRONKPOOL_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# NOTE: What differs between platforms (the threads) is written once per
#       platform, in src/platform/<platform>/: only the one of the target is built
list(FILTER KRONKPOOL_SOURCES EXCLUDE REGEX "/src/platform/")

if(WIN32)
    set(KRONKPOOL_PLATFORM win32)
else()
    set(KRONKPOOL_PLATFORM posix)
endif()

file(GLOB KRONKPOOL_PLATFORM_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/${KRONKPOOL_PLATFORM}/*.c")
list(APPEND KRONKPOOL_SOURCES ${KRONKPOOL_PLATFORM_SOURCES})

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(kronkpool-obj OBJECT ${KRONKPOOL_SOURCES})

target_include_directories(kronkpool-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    # NOTE: No /O2 here: CMake gives it in Release, and it cannot go with the /RTC1 of Debug.
    #       <stdatomic.h> needs /experimental:c11atomics (Visual Studio 2022 17.5 and later)
    target_compile_options(kronkpool-obj PRIVATE
        /W4
        /experimental:c11atomics
    )
else()
    target_compile_options(kronkpool-obj PRIVATE
        -Wall
        -Wextra
        -O3
    )
endif()

if(WIN32)
    target_compile_definitions(kronkpool-obj PRIVATE WIN32_LEAN_AND_MEAN)
endif()

set_target_properties(kronkpool-obj PROPERTIES
    C_STANDARD 11
    POSITION_INDEPENDENT_CODE ON
)

add_library(kronkpool-static STATIC $<TARGET_OBJECTS:kronkpool-obj>)
target_link_libraries(kronkpool-static PUBLIC kronkpool-obj) 

add_library(kronkpool-shared SHARED $<TARGET_OBJECTS:kronkpool-obj>)
target_link_libraries(kronkpool-shared PUBLIC kronkpool-obj)

target_compile_definitions(kronkpool-shared PRIVATE kronkpool_BUILD_DLL)

# NOTE: On Windows, a DLL only gives what it exports: the API is not marked for
#       it (the objects are the same as the static library's), so it exports all
set_target_properties(kronkpool-shared PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

add_library(kronkpool ALIAS kronkpool-static)

set_target_properties(kronkpool-static PROPERTIES
    OUTPUT_NAME kronkpool
)

set_target_properties(kronkpool-shared PROPERTIES
    OUTPUT_NAME kronkpool
)

if(MSVC)
    # NOTE: The import library of the DLL is already kronkpool.lib
    set_target_properties(kronkpool-static PROPERTIES OUTPUT_NAME kronkpool_static)
endif()

# NOTE: Installation

include(GNUInstallDirs)

install(TARGETS kronkpool-static kronkpool-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronkpool
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(KRONKPOOL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
