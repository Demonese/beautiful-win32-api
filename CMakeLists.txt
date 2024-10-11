cmake_minimum_required(VERSION 3.28)
project("beautiful_win32_api" CXX)

option(beautiful_win32_api_enable_tests "enable beautiful_win32_api tests" OFF)

set(M_LIB_OPTS_NAME "beautiful_win32_api_options")

add_library(${M_LIB_OPTS_NAME} INTERFACE)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	message(STATUS "beautiful_win32_api: MSVC")
	target_compile_options(${M_LIB_OPTS_NAME} INTERFACE "/utf-8" "/W4" "/sdl")
endif ()

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
target_link_libraries(${M_LIB_NAME} PRIVATE ${M_LIB_OPTS_NAME})

if (beautiful_win32_api_enable_tests)
	set(M_TEST_NAME "beautiful_win32_api_tests")

	add_executable(${M_TEST_NAME})
	target_compile_features(${M_TEST_NAME} PRIVATE cxx_std_20)
	target_sources(${M_TEST_NAME} PRIVATE "tests/test.cpp" "tests/dpi_awareness.manifest")
	target_link_libraries(${M_TEST_NAME} PRIVATE ${M_LIB_OPTS_NAME} ${M_LIB_NAME})
endif ()
