cmake_minimum_required(VERSION 3.26)
project(crust VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/lib/*.c")

add_executable(crust ${sources})
target_compile_options(crust PRIVATE
    -ggdb3
    -Wall -Werror -Wextra
    -Wundef
    -Wcast-align
    -Wsign-conversion
    -Wformat=2
    -Wconversion
    -Wshadow

    -Werror=nonnull
    -Werror=address
    -Werror=init-self
    -Werror=uninitialized
    -Werror=return-type
    -Werror=pointer-arith
    -Werror=implicit-fallthrough
    -Werror=missing-include-dirs
    -Werror=string-compare
    -Werror=switch
    -Werror=switch-enum
    -Werror=write-strings

    -Werror=missing-field-initializers

    -fsanitize=address)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(crust PRIVATE
        -Wlogical-op
        -Werror=invalid-memory-model
        -Werror=maybe-uninitialized
        -Werror=return-local-addr)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(crust PRIVATE
        -Werror=dangling
        -Werror=return-stack-address)
endif()

target_precompile_headers(crust PRIVATE "${CMAKE_SOURCE_DIR}/include/utils.h")

target_link_options(crust PRIVATE -fsanitize=address)
target_include_directories(crust PUBLIC "${CMAKE_SOURCE_DIR}/include")
