project(xmppsc-client)

cmake_minimum_required(VERSION 2.8)

include(GNUInstallDirs)

# Check for C++11 standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DCOMPILER_SUPPORTS_CXX11")
else()
# Until now we are fine without CXX11 support. Change here later if there are features we cannot circumvent
#  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


# Build the library
FILE(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(xmppsc-client SHARED ${sources})

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(xmppsc-client ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(xmppsc-client ${CONFIG_LIBRARY})


# Installation stuff
INSTALL(TARGETS xmppsc-client 
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

FILE(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
INSTALL(FILES ${headers}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmppsc
)
INSTALL(FILES spacecontrol.config.example
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
)
