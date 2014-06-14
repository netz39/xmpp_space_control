project(i3c_cli)

cmake_minimum_required(VERSION 2.8)

# Check for C++11 standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DCOMPILER_SUPPORTS_CXX11")
else()
  message(WARNING "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++0x")
endif()

add_executable(i3c_cli main.cpp ${client_sources})

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(i3c_cli ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(i3c_cli ${CONFIG_LIBRARY})

find_library(XMPPSC_LIBRARY xmppsc-client)
target_link_libraries(i3c_cli ${XMPPSC_LIBRARY})
