cmake_minimum_required(VERSION 4.0)
project(blankwindow)

set(SDLGL_BUILD_EXAMPLES OFF)
add_subdirectory(../../ library)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    sdlgl
)
