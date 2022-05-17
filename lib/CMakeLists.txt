# Loop through all sub directories and add them
file(GLOB sub-dirs ${CMAKE_CURRENT_LIST_DIR}/*)
foreach(dir ${sub-dirs})
    IF(IS_DIRECTORY ${dir})
        add_subdirectory(${dir})
    ENDIF()
endforeach()