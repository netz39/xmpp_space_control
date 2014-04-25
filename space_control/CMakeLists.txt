project(xmpp_space_control)

cmake_minimum_required(VERSION 2.8)

# Check for C++11 standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DCOMPILER_SUPPORTS_CXX11")
else()
# Until now we are fine without CXX11 support. Change here later if there are features we cannot circumvent
#  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


add_executable(xmpp_space_control main.cpp configuredclientfactory.cpp spacecontrolclient.cpp)

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(xmpp_space_control ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(xmpp_space_control ${CONFIG_LIBRARY})
