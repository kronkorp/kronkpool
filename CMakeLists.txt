cmake_minimum_required(VERSION 3.15)
project(kronkpool LANGUAGES C)

include(cmake/ProjectOptions.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/Utils.cmake)

option(KRONKPOOL_BUILD_TESTS "Build kronkpool tests" ON)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

include(FetchContent)

FetchContent_Declare(
    kronklab
    GIT_REPOSITORY https://github.com/kronkorp/kronklab
    GIT_TAG        main
)

FetchContent_MakeAvailable(kronklab)

file(GLOB_RECURSE KRONKPOOL_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(kronkpool-obj OBJECT ${KRONKPOOL_SOURCES})

target_include_directories(kronkpool-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(kronkpool-obj PRIVATE
    -Wall
    -Wextra
    -O3
)

set_target_properties(kronkpool-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

add_library(kronkpool-static STATIC $<TARGET_OBJECTS:kronkpool-obj>)
target_link_libraries(kronkpool-static PUBLIC kronkpool-obj) 

add_library(kronkpool-shared SHARED $<TARGET_OBJECTS:kronkpool-obj>)
target_link_libraries(kronkpool-shared PUBLIC kronkpool-obj)

target_compile_definitions(kronkpool-shared PRIVATE kronkpool_BUILD_DLL)

add_library(kronkpool ALIAS kronkpool-static)

set_target_properties(kronkpool-static PROPERTIES
    OUTPUT_NAME kronkpool
)

set_target_properties(kronkpool-shared PROPERTIES
    OUTPUT_NAME kronkpool
)

# NOTE: Installation

include(GNUInstallDirs)

install(TARGETS kronkpool-static kronkpool-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronkpool
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(KRONKPOOL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/units)
endif()
