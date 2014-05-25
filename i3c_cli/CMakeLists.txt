project(i3c_cli)

cmake_minimum_required(VERSION 2.8)

# Check for C++11 standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DCOMPILER_SUPPORTS_CXX11")
else()
  message(ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

file(GLOB client_sources "${CMAKE_CURRENT_SOURCE_DIR}/../space_control/*.cpp")
file(GLOB i3c_sources "${CMAKE_CURRENT_SOURCE_DIR}/../i3c_client/*.cpp")
list(REMOVE_ITEM i3c_sources "${CMAKE_CURRENT_SOURCE_DIR}/../i3c_client/main.cpp")
list(REMOVE_ITEM i3c_sources "${CMAKE_CURRENT_SOURCE_DIR}/../i3c_client/i2cendpoint_dummy.cpp")
list(REMOVE_ITEM i3c_sources "${CMAKE_CURRENT_SOURCE_DIR}/../i3c_client/i2cendpoint_wiringpi.cpp")

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

list(APPEND i3c_sources "${CMAKE_CURRENT_SOURCE_DIR}/../i3c_client/i2cendpoint_${I2C_ENDPOINT_IMPL}.cpp")

add_executable(i3c_cli main.cpp 
			${client_sources}
			${i3c_sources})

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(i3c_cli ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(i3c_cli ${CONFIG_LIBRARY})

# if wiringPi has been included
if (I2C_ENDPOINT_IMPL STREQUAL "wiringpi")
  # add link targets for the wiringPi libraries
  # WIRINGPI_LIBRARY has already been checked before
  target_link_libraries(i3c_cli ${WIRINGPI_LIBRARY})
endif (I2C_ENDPOINT_IMPL STREQUAL "wiringpi")
