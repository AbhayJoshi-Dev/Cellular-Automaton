cmake_minimum_required(VERSION 3.25.0)

project(Cellular-Automaton)

set(DEPS_DIR ${CMAKE_SOURCE_DIR}/build/external)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(external/SDL)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)
  
add_executable(Cellular-Automaton ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Cellular-Automaton PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
                                            ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL/include/)

target_link_libraries(Cellular-Automaton SDL2-static SDL2main)