# NOTE: Included from the root CMakeLists.txt with -DKRONKPOOL_BUILD_TESTS=ON

# NOTE: The smoke test runs on every platform, against the static and the shared library
add_executable(kronkpool_smoke smoke/smoke.c)
target_link_libraries(kronkpool_smoke PRIVATE kronkpool-static)
add_test(NAME kronkpool_smoke COMMAND kronkpool_smoke)

add_executable(kronkpool_smoke_shared smoke/smoke.c)
target_link_libraries(kronkpool_smoke_shared PRIVATE kronkpool-shared)
add_test(NAME kronkpool_smoke_shared COMMAND kronkpool_smoke_shared)

set_tests_properties(kronkpool_smoke kronkpool_smoke_shared PROPERTIES TIMEOUT 30)

if(WIN32)
    # NOTE: Windows looks for a DLL next to the executable
    add_custom_command(TARGET kronkpool_smoke_shared POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:kronkpool-shared> $<TARGET_FILE_DIR:kronkpool_smoke_shared>
    )

    # NOTE: kronklab runs each test in a fork(), which Windows does not have
    return()
endif()

# NOTE: Pinned: the tests do not change when kronklab's main does
include(FetchContent)

FetchContent_Declare(
    kronklab
    GIT_REPOSITORY https://github.com/kronkorp/kronklab
    GIT_TAG        ed591c8d0172558ca72eecac4f55c2bb1344625a
)

FetchContent_MakeAvailable(kronklab)

add_subdirectory(units)
