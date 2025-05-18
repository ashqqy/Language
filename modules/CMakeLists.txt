cmake_minimum_required(VERSION 3.10)
project(modules LANGUAGES C)

add_library(modules STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/tree.c
    ${CMAKE_CURRENT_SOURCE_DIR}/common.c
)

target_include_directories(modules PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

install(TARGETS modules
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/modules
)
