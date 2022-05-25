cmake_minimum_required(VERSION 3.22)

project(sfml-template LANGUAGES CXX)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++20")

# souces lib name
set(SOURCES_LIB sources)
add_subdirectory(src)
add_subdirectory(include)

add_executable(${PROJECT_NAME} src/main.cpp "include/Game/Timer.h" "include/Game/bla.h")
target_link_libraries(${PROJECT_NAME} PUBLIC ${SOURCES_LIB})

add_subdirectory(lib)
add_subdirectory(resources)

