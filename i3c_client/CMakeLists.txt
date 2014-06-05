project(i3c_client)

cmake_minimum_required(VERSION 2.8)

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
				   i3cmethods.cpp
						  ../space_control/configuredclientfactory.cpp
                                                  ../space_control/spacecontrolclient.cpp
                                                  ../space_control/methodhandler.cpp
                                                  ../space_control/accessfilter.cpp
                                                  ../space_control/daemon.cpp)

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(i3c_client ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(i3c_client ${CONFIG_LIBRARY})

# if wiringPi has been included
if (I2C_ENDPOINT_IMPL STREQUAL "wiringpi")
  # add link targets for the wiringPi libraries
  # WIRINGPI_LIBRARY has already been checked before
  target_link_libraries(i3c_client ${WIRINGPI_LIBRARY})
endif (I2C_ENDPOINT_IMPL STREQUAL "wiringpi")
