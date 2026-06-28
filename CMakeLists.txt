cmake_minimum_required(VERSION 4.0)
project(sdlgl)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (SDLGL_BUILD_EXAMPLES)
    add_subdirectory(examples)
else()
    find_package(OpenGL REQUIRED)
    add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

    add_library(${PROJECT_NAME} STATIC
        src/sdlgl.cpp
        lib/glad.c
    )

    target_link_libraries(${PROJECT_NAME}
        SDL3::SDL3
        OpenGL::GL
    )

    target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
    )
endif()
