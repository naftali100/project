cmake_minimum_required(VERSION 3.22)

project(sfml-template LANGUAGES CXX)

if(!MSVC)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++20 -g")
endif()

include(cmake/conf.cmake)


# souces lib name
set(SOURCES_LIB sources)
add_subdirectory(src)
add_subdirectory(include)

add_executable(${PROJECT_NAME} src/main.cpp "include/Game/States/GameStateInitializer.h")
target_link_libraries(${PROJECT_NAME} PUBLIC ${SOURCES_LIB})

add_subdirectory(lib)

add_subdirectory(resources)
