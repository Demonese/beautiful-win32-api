cmake_minimum_required(VERSION 3.28)
project("beautiful_win32_api" CXX)

set(M_LIB_NAME "beautiful_win32_api")

file(GLOB_RECURSE M_PUBLIC RELATIVE ${CMAKE_CURRENT_LIST_DIR} "headers/*.hpp")
file(GLOB_RECURSE M_PRIVATE RELATIVE ${CMAKE_CURRENT_LIST_DIR} "sources/*.hpp" "sources/*.cpp")
message(STATUS "M_PUBLIC: ${M_PUBLIC}")
message(STATUS "M_PRIVATE: ${M_PRIVATE}")

add_library(${M_LIB_NAME} STATIC)
target_compile_features(${M_LIB_NAME} PRIVATE cxx_std_20)
target_include_directories(${M_LIB_NAME} PUBLIC headers)
target_include_directories(${M_LIB_NAME} PRIVATE sources)
target_sources(${M_LIB_NAME} PRIVATE ${M_PUBLIC} ${M_PRIVATE})

set(M_TEST_NAME "test")

add_executable(${M_TEST_NAME})
target_compile_features(${M_TEST_NAME} PRIVATE cxx_std_20)
target_sources(${M_TEST_NAME} PRIVATE "tests/test.cpp")
target_link_libraries(${M_TEST_NAME} PRIVATE ${M_LIB_NAME})
