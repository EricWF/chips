cmake_minimum_required (VERSION 2.7)

################################################################################
#                   Toolchain config
################################################################################


# Misc
option(CONFIG_ALL_WARNINGS "Add warnings for GCC/Clang" ON)
option(CONFIG_UNIT_TESTS "build unit tests" ON)


        
################################################################################
#                         PROJECT Chips
################################################################################

# need C to get the pthreads library 
project(Chips CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/modules")
add_definitions( "-DCHIPS_RESOURCE_ROOT=\"${CMAKE_CURRENT_LIST_DIR}/res/\"" )
add_definitions( "-DCHIPS_LEVEL_ROOT=\"${CMAKE_CURRENT_LIST_DIR}/levels/\"")
add_definitions( "-DCHIPS_SFML_WORKAROUNDS")

################################################################################
#                       Toolchain config (Post-project)
################################################################################

set( CMAKE_BUILD_TYPE DEBUG )

if ( WIN32 )
    message( FATAL_ERROR "Windows not supported!" )
endif()

if ( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" )
    message(STATUS "Configuring for GCC")
    set( CXX_COMPILER_GCC TRUE )
    set( CXX_COMPILER_CLANG FALSE )
elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
    message(STATUS "Configuring for Clang")
    set( CXX_COMPILER_GCC FALSE )
    set( CXX_COMPILER_CLANG TRUE )
else()
    message( FATAL_ERROR "Compiler not supported! (${CMAKE_CXX_COMPILER_ID})" )
endif()

set(CXX_VERSION_FLAGS " -std=c++11 ")


set(CXX_WARNING_FLAGS " -Wall -Wextra -pedantic ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_VERSION_FLAGS} ${CXX_WARNING_FLAGS}")



if ( CXX_COMPILER_GCC )
    # check min version
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.8.0")
        message(FATAL_ERROR "Insufficient GCC Version")
    elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.9.0")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fdiagnostics-color=auto ")
    endif()
    # Check C++1y config
    if (CONFIG_CXX1Y AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9.0")
        message(FATAL_ERROR "Insufficient GCC Version for C++1y")
    endif()
    set(CXX_GCC_FLAGS " -fmax-errors=3 " )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_GCC_FLAGS}")
elseif( CXX_COMPILER_CLANG )
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.3")
        message(FATAL_ERROR "Insufficient Clang Version")
    endif()
    if (CONFIG_CXX1Y AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "3.4")
        message(FATAL_ERROR "Insufficient Clang Version for C++1y")
    endif()
    set(CXX_CLANG_FLAGS " -ferror-limit=3 " )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CLANG_FLAGS}")
endif()


set( CMAKE_CXX_FLAGS_RELEASE "-O3  -DNDEBUG " )
set( CMAKE_C_FLAGS_RELEASE "-O3  -DNDEBUG " )
set( CMAKE_C_FLAGS_DEBUG " -g -fno-omit-frame-pointer -DELIB_WARN_ON " )
set( CMAKE_CXX_FLAGS_DEBUG " -g -fno-omit-frame-pointer  -DDELIB_WARN_ON " )

################################################################################
#                          Build & Installation Config
################################################################################

set(headerDir ${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/elib)
set(ELIB_LIBRARIES_DIR "${CMAKE_CURRENT_LIST_DIR}/elib/bin")
set(ELIB_LIBRARIES "-L${ELIB_LIBRARIES_DIR} -Wl,-rpath -Wl,${ELIB_LIBRARIES_DIR} -lelib")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

################################################################################
#                           Macros & Functions
################################################################################

macro(set_dot_dir)
    set(dotDir ${CMAKE_CURRENT_LIST_DIR})
endmacro(set_dot_dir)

macro(add_library_src)
    set_dot_dir()
    aux_source_directory(${dotDir} _src)
    list(APPEND LIBRARY_SRC ${_src})
endmacro(add_library_src)

macro(add_unit_test_src)
    set_dot_dir()
    aux_source_directory(${dotDir}/unit_tests _test)
    list(APPEND UNIT_TEST_SRC ${_test})
endmacro(add_unit_test_src)

macro(add_src)
    add_library_src()
    add_unit_test_src()
endmacro(add_src)

macro(stringify_list m_list dest)
    string(REPLACE ";" " " ${dest} "${m_list}")
endmacro(stringify_list)

macro(add_flags to)
    stringify_list("${ARGN}" _TMP)
    set(${to} "${${to}} ${_TMP}")
endmacro(add_flags)

################################################################################
#
################################################################################

add_subdirectory(src)


