cmake_minimum_required (VERSION 3.5)
project (matrix_lib)

# Lib
add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC
        ./src
)
