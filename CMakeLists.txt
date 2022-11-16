cmake_minimum_required(VERSION 3.21.2)

project(Cellular-Automaton)

set(DEPS_DIR ${CMAKE_SOURCE_DIR}/build/external)

add_subdirectory(external/SDL)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)
  
add_executable(Cellular-Automaton ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(Cellular-Automaton PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
                                            ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL/include/)

target_link_libraries(Cellular-Automaton SDL2 SDL2main)

add_custom_command(TARGET Cellular-Automaton POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${DEPS_DIR}/SDL2/Debug/SDL2d.dll"
                    $<TARGET_FILE_DIR:Cellular-Automaton>)

add_custom_command(TARGET Cellular-Automaton POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:Cellular-Automaton>/res)