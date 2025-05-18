cmake_minimum_required(VERSION 3.10)
project(frontend LANGUAGES C)

add_library(frontend STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tokenization.c
)

target_include_directories(frontend PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(frontend PUBLIC
    modules
)

install(TARGETS frontend
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/frontend
)
