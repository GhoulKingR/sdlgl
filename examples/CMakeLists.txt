cmake_minimum_required(VERSION 4.0)
project(examples)

if(BLANK_WINDOW)
    add_subdirectory(blank_window)
elseif(IMGUI_USAGE)
    add_subdirectory(imgui_usage)
endif()
