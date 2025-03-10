cmake_minimum_required(VERSION 3.19)

file(GLOB_RECURSE IMGUI_SOURCES *.cpp)
file(GLOB_RECURSE IMGUI_HEADERS *.h *.hpp)

add_library(ImGUI STATIC
        ${IMGUI_SOURCES} ${IMGUI_HEADERS}
)
target_include_directories(ImGUI PUBLIC ./)
target_link_libraries(ImGUI PUBLIC glfw freetype)
target_compile_features(ImGUI PRIVATE cxx_std_20)