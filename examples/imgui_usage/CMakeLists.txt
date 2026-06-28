cmake_minimum_required(VERSION 4.0)
project(imgui_usage)

set(SDLGL_BUILD_EXAMPLES OFF)
add_subdirectory(../../ library)

add_executable(${PROJECT_NAME}
    main.cpp

    lib/imgui/imgui.cpp
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_impl_opengl3.cpp
    lib/imgui/imgui_impl_sdl3.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
)

target_link_libraries(${PROJECT_NAME}
    sdlgl
)

target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include"
)
