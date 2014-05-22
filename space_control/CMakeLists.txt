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
file(GLOB sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(xmppsc-client SHARED ${sources})

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(xmppsc-client ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(xmppsc-client ${CONFIG_LIBRARY})


# Installation stuff
install(TARGETS xmppsc-client 
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${headers}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xmppsc
)
install(FILES spacecontrol.config.example
  DESTINATION ${CMAKE_INSTALL_DOCDIR}
)


# Packaging
# see http://www.cmake.org/Wiki/CMake:Packaging_With_CPack#Using_CMake_variables_to_configure_CPack

include(InstallRequiredSystemLibraries)

# Generate Debian packages
#  It seems that the debian generator cannot create different debian packages from one CMake source.
#  (Components do not solve the problem.)
set(CPACK_GENERATOR "DEB")


set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "XMPP SpaceControl client library")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Stefan Haun <tux@netz39.de>")
# TODO create those files
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
#  set(CPACK_STRIP_FILES "bin/MyExecutable")
#  set(CPACK_SOURCE_STRIP_FILES "")
#set(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")
include(CPack)

