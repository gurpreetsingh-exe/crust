cmake_minimum_required(VERSION 3.26)
project(crust VERSION 0.1)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(ENABLE_TESTS OFF CACHE BOOL "Enable tests")

set(test_driver "${CMAKE_SOURCE_DIR}/lib/driver/test_driver.cc")

file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/lib/*.cc")
list(FILTER sources EXCLUDE REGEX "/tests/")
list(REMOVE_ITEM sources ${test_driver})

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
    -Werror=write-strings

    -Werror=missing-field-initializers
    -Werror=non-virtual-dtor
    -Werror=pessimizing-move

    -fsanitize=address)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(crust PRIVATE
        -Wlogical-op
        -Werror=invalid-memory-model
        -Werror=maybe-uninitialized
        -Werror=return-local-addr)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(crust PRIVATE
        -Werror=dangling
        -Werror=return-stack-address)
endif()

add_subdirectory(third_party/fmt)
target_link_libraries(crust fmt)

target_precompile_headers(crust PRIVATE "${CMAKE_SOURCE_DIR}/include/utils.hh")

target_link_options(crust PRIVATE -fsanitize=address)
target_include_directories(crust PUBLIC "${CMAKE_SOURCE_DIR}/include")

if(ENABLE_TESTS)
    message("-- tests enabled")
    set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
    file(GLOB_RECURSE test_sources "${CMAKE_SOURCE_DIR}/lib/*.cc")
    list(FILTER test_sources INCLUDE REGEX "/tests/")

    add_subdirectory(third_party/googletest)
    enable_testing()
    include(GoogleTest)

    add_executable(run_tests ${test_driver})
    target_include_directories(run_tests PUBLIC "${CMAKE_SOURCE_DIR}/include")
    target_link_libraries(run_tests GTest::gtest)

    gtest_discover_tests(run_tests)
endif()
