# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
# cmake --build build
# ./build/Language ./Tests/Factorial.mylang

cmake_minimum_required(VERSION 3.10)
project(Language LANGUAGES C)

# Output dirs
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(frontend)
# add_subdirectory(middleend)
# add_subdirectory(backend)
add_subdirectory(modules)

# Главная программа
add_executable(Language
    main.c
)

target_link_libraries(Language PRIVATE
    frontend
    # MiddleEnd
    # backend
    modules
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Language PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )
    target_link_options(Language PRIVATE
        -fsanitize=address
        -fsanitize=undefined
    )
endif()

install(TARGETS Language DESTINATION bin)
