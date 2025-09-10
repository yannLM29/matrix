cmake_minimum_required (VERSION 3.16)
project (matrix_lib)

# Tests
find_package(Catch2 3 REQUIRED)

set(TEST_DIR ./tests)
set(TEST_FILES
        ${TEST_DIR}/tests.cpp
)
add_executable(${PROJECT_NAME}_tests
        ${TEST_FILES}
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(${PROJECT_NAME}_tests PUBLIC ./src)

include(CTest)
include(Catch)
catch_discover_tests(${PROJECT_NAME}_tests)