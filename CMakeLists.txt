cmake_minimum_required(VERSION 3.0)
project(EventLoop)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
            COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif()

set(SOURCE_FILES
        EventLoop.cpp
        EventLoop.h)

#include(doctest/scripts/cmake/common.cmake)

add_library(EventLoop SHARED ${SOURCE_FILES})