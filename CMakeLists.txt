cmake_minimum_required(VERSION 3.10)
project(writex LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

add_library(${PROJECT_NAME} SHARED
    logger.cpp
)

add_executable(main
    main.cpp
)

target_link_libraries(main PRIVATE ${PROJECT_NAME})

add_custom_target(run
    COMMAND main
    DEPENDS ${PROJECT_NAME}
)