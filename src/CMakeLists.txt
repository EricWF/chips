# Author: Eric Fiselier
# Compiler: GCC/Clang
# 
# I apologize to windows users, I have built it this project on windows,
# but the project configurations in this make file DO NOT WORK and they 
# must be commented out.

set(LIBRARY_FLAGS "")
set(UNIT_TEST_FLAGS "")

# Probably uneeded
# add_flags(LIBRARY_FLAGS -fPIC)

if (CONFIG_ALL_WARNINGS)

    add_flags( LIBRARY_FLAGS 
    -Wall  -pedantic  
    -pedantic-errors -Wextra  -Wall -Waggregate-return -Wcast-align 
    -Wcast-qual  -Wchar-subscripts  -Wcomment -Wconversion 
    -Wdisabled-optimization -Wfloat-equal  -Wformat  -Wformat=2 
    -Wformat-nonliteral -Wformat-security  -Wformat-y2k -Wimport  -Winit-self  
    -Winline -Winvalid-pch -Wlong-long -Wmissing-braces 
    -Wmissing-field-initializers -Wmissing-format-attribute 
    -Wmissing-include-dirs -Wmissing-noreturn -Wpacked  -Wpadded -Wparentheses
    -Wpointer-arith -Wredundant-decls -Wreturn-type -Wsequence-point  -Wshadow 
    -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2 
    -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs  -Wuninitialized 
    -Wunknown-pragmas  -Wunreachable-code -Wunused -Wunused-function  
    -Wunused-label  -Wunused-parameter -Wunused-value  -Wunused-variable  
    -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings
  )
  
  if (CXX_COMPILER_GCC)
    add_flags( LIBRARY_FLAGS -Wunsafe-loop-optimizations )
    #add_flags( LIBRARY_FLAGS -Weffc++)
  elseif (CXX_COMPILER_CLANG)
    add_flags( LIBRARY_FLAGS
      -Weverything -Wno-c++98-compat 
      -Wno-c++98-compat-pedantic -Wno-newline-eof
    )
  endif()
  
  add_flags( LIBRARY_FLAGS 
    -Wno-padded -Wno-aggregate-return -Wno-inline -Wno-long-long
  )
  
endif()


include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)


set(LIBRARY_SRC "")
set(UNIT_TEST_SRC "")

# compile into objects
include(chips/build.cmake)

add_library(ObjLib OBJECT ${LIBRARY_SRC})
set_target_properties(ObjLib PROPERTIES COMPILE_FLAGS "${LIBRARY_FLAGS}")

################################################################################
  #create static & shared libraries

find_package( Threads REQUIRED )

add_executable(chips main.cpp $<TARGET_OBJECTS:ObjLib>)

target_link_libraries(chips ${SDL2_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} 
                        	${STDLIB_LINK})
  
message(STATUS "${SDL2_LIBRARIES}")

################################################################################

if(CONFIG_UNIT_TESTS) 

	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_MULTITHREADED OFF)
	set(Boost_USE_STATIC_RUNTIME OFF)
	find_package(Boost COMPONENTS unit_test_framework REQUIRED)
	
	include_directories(${Boost_INCLUDE_DIRS})
	add_library(TestObj OBJECT ${UNIT_TEST_SRC} unit_test_main.cpp)
	
  add_executable(chips_test $<TARGET_OBJECTS:TestObj> $<TARGET_OBJECTS:ObjLib>)


 	target_link_libraries(chips_test ${SDL2_LIBRARIES}
    ${STDLIB_LINK} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES}
   )
   
endif()



