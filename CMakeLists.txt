cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(writex LANGUAGES CXX)

# CONFIGURATION

if(${CMAKE_BUILD_TYPE} STREQUAL "TSan")
    set(SANITIZER thread)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "ASan")
    set(SANITIZER address)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "LSan")
    set(SANITIZER leak)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "MSan")
    set(SANITIZER memory)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "USan")
    set(SANITIZER undefined)
endif()

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# FetchContent part

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.14.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# LIBRARY

add_library(${PROJECT_NAME} SHARED
    src/writex.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/WriteX)

# TESTING

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_testing()
    add_executable(tests
        tests/test_main.cpp
    )
    target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR}/include/WriteX)
    target_link_libraries(tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

    if(NOT DEFINED ${SANITIZER} AND ${SANITIZER} STREQUAL "")
        target_compile_options(tests PRIVATE -fsanitize=${SANITIZER} -g)
        target_link_options(tests PRIVATE -fsanitize=${SANITIZER})
    endif()

    add_test(
        NAME General
        COMMAND tests
    )
endif()
