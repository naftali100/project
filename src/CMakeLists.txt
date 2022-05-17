file(GLOB_RECURSE source_files "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
list(REMOVE_ITEM source_files "${CMAKE_CURRENT_LIST_DIR}/main.cpp")

add_library(${SOURCES_LIB} OBJECT ${source_files})
target_link_libraries(${SOURCES_LIB} PUBLIC plog)

target_precompile_headers(${SOURCES_LIB} PUBLIC "${CMAKE_SOURCE_DIR}/include/pch.h")

if( WIN32 ) 
    # dosnt work on linux from package manager (need to manualy compile dynamic lib)...
    set (SFML_STATIC_LIBRARIES TRUE)
    set (SFML_DIR "C:/SFML/SFML-2.5.1/lib/cmake/SFML")
    # required for audio
    configure_file ("C:/SFML/SFML-2.5.1/bin/openal32.dll" ${CMAKE_BINARY_DIR} COPYONLY)
endif()

find_package(OpenGL REQUIRED)
find_package (SFML 2.5 COMPONENTS graphics audio network REQUIRED)

if( WIN32 ) 
    # sfml-main - is windows only 
    set_target_properties (sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
    target_link_libraries (${SOURCES_LIB} PUBLIC sfml-system sfml-graphics sfml-audio sfml-main sfml‑network ${OPENGL_LIBRARIES})
else()
    set_target_properties (sfml-system sfml-window sfml-graphics sfml-audio sfml-network PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
    target_link_libraries (${SOURCES_LIB} PUBLIC sfml-system sfml-graphics sfml-audio sfml-network ${OPENGL_LIBRARIES})
endif()