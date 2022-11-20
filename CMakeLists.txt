cmake_minimum_required(VERSION 3.25.0)

project(Cellular-Automaton)

set(DEPS_DIR ${CMAKE_SOURCE_DIR}/build/external)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/SDL)

include_directories(${CMAKE_SOURCE_DIR}/external/SDL/include/)

set(IMGUI_SOURCE_FILES
  external/imgui/imgui.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_impl_sdlrenderer.cpp
  external/imgui/imgui_impl_sdl.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp)

set(IMGUI_HEADER_FILES
  external/imgui/imconfig.h
  external/imgui/imgui.h
  external/imgui/imgui_internal.h
  external/imgui/imstb_rectpack.h
  external/imgui/imstb_textedit.h
  external/imgui/imstb_truetype.h
  external/imgui/imgui_impl_sdlrenderer.h
  external/imgui/imgui_impl_sdl.h)

add_library(imgui STATIC ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})
target_include_directories(imgui PUBLIC external/imgui/)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)
  
add_executable(Cellular-Automaton ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Cellular-Automaton PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_libraries(Cellular-Automaton imgui SDL2-static SDL2main)