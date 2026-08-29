file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_executable(kronkpool-unit-tests ${SOURCES})

target_include_directories(kronkpool-unit-tests PRIVATE
	${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(kronkpool-unit-tests PRIVATE kronklab kronkpool)
apply_project_settings(kronkpool-unit-tests)

add_test(NAME kronkpool-unit-tests COMMAND kronkpool-unit-tests)
