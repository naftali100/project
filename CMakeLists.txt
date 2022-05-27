cmake_minimum_required(VERSION 3.22)

project(sfml-template LANGUAGES CXX)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++20")

# tzvi add it for checking
##################################################################################3333
if (MSVC AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
    set(CMAKE_CXX_STANDARD 23)
else ()
    set(CMAKE_CXX_STANDARD 20)
endif ()
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
if (MSVC)
    add_compile_options (/W4 /permissive- /Zc:externConstexpr /Zc:inline /analyze)
endif ()
##################################################################################

# souces lib name
set(SOURCES_LIB sources)
add_subdirectory(src)
add_subdirectory(include)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SOURCES_LIB})

add_subdirectory(lib)
add_subdirectory(resources)
