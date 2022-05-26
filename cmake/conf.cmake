# New CMake versions map C++20 to /std:c++20 instead of c++latest, so <format>
# and other unstable ABI features aren't supported. Use C++23 for such versions
if (MSVC AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.21)
    set(CMAKE_CXX_STANDARD 23)
else ()
    set(CMAKE_CXX_STANDARD 20)
endif ()
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)


# used for enabling additional compiler options if supported
include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag flag)
    check_cxx_compiler_flag("${flag}" flag_supported)
    if(flag_supported)
        # add_compile_options("${flag}")
        target_compile_options(${SOURCES_LIB} PUBLIC "${flag}")
    endif()
    unset(flag_supported CACHE)
endfunction()



if (MSVC)
    add_compile_options (/std:c++17 /std:c++20 /std:c++latest)
    # add_compile_options (/W4 /permissive- /Zc:externConstexpr /Zc:inline /analyze
    #     /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
    #     /w14263 # 'function': member function does not override any base class virtual member function
    #     /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not
    #             # be destructed correctly
    #     /w14287 # 'operator': unsigned/negative constant mismatch
    #     /w14296 # 'operator': expression is always 'boolean_value'
    #     /w14545 # expression before comma evaluates to a function which is missing an argument list
    #     /w14546 # function call before comma missing argument list
    #     /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
    #     /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
    #     /w14555 # expression has no effect; expected expression with side-effect
    #     /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
    #     /w14905 # wide string literal cast to 'LPSTR'
    #     /w14906 # string literal cast to 'LPWSTR'
    #     /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
    # )
else ()
    # enable_cxx_compiler_flag("-Wall")
    # enable_cxx_compiler_flag("-Wextra") # reasonable and standard
    # enable_cxx_compiler_flag("-Wshadow") # warn the user if a variable declaration shadows one from a parent context
    # enable_cxx_compiler_flag("-Wnon-virtual-dtor") # warn the user if a class with virtual functions has a non-virtual destructor
    # enable_cxx_compiler_flag("-Wold-style-cast") # warn for c-style casts
    # enable_cxx_compiler_flag("-Wcast-align") # warn for potential performance problem casts
    # enable_cxx_compiler_flag("-Wunused") # warn on anything being unused
    # enable_cxx_compiler_flag("-Woverloaded-virtual") # warn if you overload (not override) a virtual function
    # enable_cxx_compiler_flag("-Wpedantic") # warn if non-standard C++ is used
    # enable_cxx_compiler_flag("-Wconversion") # warn on type conversions that may lose data
    # enable_cxx_compiler_flag("-Wsign-conversion") # warn on sign conversions
    # enable_cxx_compiler_flag("-Wnull-dereference") # warn if a null dereference is detected
endif ()
