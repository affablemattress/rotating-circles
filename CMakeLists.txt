# CMakeList.txt : CMake project for ras-rotating-circles, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ras-rotating-circles")

add_executable (ras-rotating-circles "main.c" "List.c" "List.h" "Circle.h" "Circle.c" "include/raygui.h")

target_include_directories(ras-rotating-circles PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(ras-rotating-circles "${PROJECT_SOURCE_DIR}/lib/raylib.lib" winmm.lib)