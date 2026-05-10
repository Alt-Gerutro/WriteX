cmake_minimum_required(VERSION 3.14)

# OPTIONS
option(WRITEX_BUILD_EXAMPLES "Build WriteX Examples" OFF)
option(WRITEX_BUILD_TESTS "Build WriteX Tests" OFF)

# PROJECT

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

# FetchContent part

if(WRITEX_BUILD_TESTS)
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

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/WriteX)

# EXAMPLES

if(WRITEX_BUILD_EXAMPLES)
    file(GLOB EXAMPLES_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/examples/*.cpp")

    foreach(src_file ${EXAMPLES_SOURCES})
        get_filename_component(src_name ${src_file} NAME_WE)
        add_executable(${src_name} ${src_file})

        target_include_directories(${src_name} PUBLIC ${PROJECT_SOURCE_DIR}/include/WriteX)
        target_link_libraries(${src_name} PRIVATE ${PROJECT_NAME})
    endforeach()
endif()

# TESTING

if(WRITEX_BUILD_TESTS)
    enable_testing()
    add_executable(tests
        tests/test_main.cpp
    )
    target_include_directories(tests PUBLIC ${PROJECT_SOURCE_DIR}/include/WriteX)
    target_link_libraries(tests PRIVATE ${PROJECT_NAME} Catch2::Catch2WithMain)

    if(DEFINED ${SANITIZER})
        if(NOT ${SANITIZER} STREQUAL "")
            target_compile_options(tests PRIVATE -fsanitize=${SANITIZER} -g)
            target_link_options(tests PRIVATE -fsanitize=${SANITIZER})
        endif()
    endif()

    add_test(
        NAME General
        COMMAND tests
    )
endif()
