cmake_minimum_required(VERSION 3.10)
project(modules LANGUAGES C)

add_library(modules STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/token.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common.c
)

target_include_directories(modules PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS modules
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/modules
)
