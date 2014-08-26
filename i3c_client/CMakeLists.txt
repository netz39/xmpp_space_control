project(i3c_client)

cmake_minimum_required(VERSION 2.8)

# Check for C++11 standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

include(GNUInstallDirs)

if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DCOMPILER_SUPPORTS_CXX11")
else()
  message(WARNING "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CSS_FLAGS} -std=c++0x")
endif()

### Check and select the I2C library
# dummy is the default
set(I2C_ENDPOINT_IMPL "dummy")
# do we have wiringPi?
find_library(WIRINGPI_LIBRARY 
	NAMES wiringPi wiringPiDev
)
if (WIRINGPI_LIBRARY)
  # use this implementation instead and add the target link libraries
  set(I2C_ENDPOINT_IMPL "wiringpi")
endif(WIRINGPI_LIBRARY)


add_executable(i3c_client main.cpp i2cmethods.cpp 
				   i2cendpoint.cpp i2cendpoint_${I2C_ENDPOINT_IMPL}.cpp
				   i3cmethods.cpp)

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(i3c_client ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(i3c_client ${CONFIG_LIBRARY})

find_library(POPT_LIBRARY popt)
target_link_libraries(i3c_client ${POPT_LIBRARY})

find_library(XMPPSC_LIBRARY xmppsc-client)
target_link_libraries(i3c_client ${XMPPSC_LIBRARY})

# if wiringPi has been included
if (I2C_ENDPOINT_IMPL STREQUAL "wiringpi")
  # add link targets for the wiringPi libraries
  # WIRINGPI_LIBRARY has already been checked before
  target_link_libraries(i3c_client ${WIRINGPI_LIBRARY})
endif (I2C_ENDPOINT_IMPL STREQUAL "wiringpi")


# Installation stuff
install(TARGETS i3c_client 
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES spacecontrol.config.example
  DESTINATION ${CMAKE_INSTALL_FULL_DOCDIR}
)

# TODO rather with ${CMAKE_INSTALL_FULL_SYSCONFDIR}
# How to set /etc as target directory?

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/resource/etc/init.d/i3cclient"
  DESTINATION /etc/init.d/
)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/resource/etc/default/i3cclient"
  DESTINATION /etc/default/
)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/resource/etc/i3cclient/spacecontrol.config"
  DESTINATION /etc/i3cclient/
)

