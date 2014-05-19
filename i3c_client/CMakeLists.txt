project(i3c_client)

cmake_minimum_required(VERSION 2.8)

### Check and select the I2C library
# dummy is the default
set(I2C_ENDPOINT_IMPL dummy)
# do we have wiredPi?
find_library(WIREDPI_LIBRARY wiredPi)
if (WIREDPI_LIBRARY)
  # use this implementation instead and add the target link libraries
  set(I2C_ENDPOINT_IMPL wiredpi)
endif(WIREDPI_LIBRARY)


add_executable(i3c_client main.cpp i2cmethods.cpp 
				   i2cendpoint.cpp i2cendpoint_${I2C_ENDPOINT_IMPL}.cpp
						  ../space_control/configuredclientfactory.cpp
                                                  ../space_control/spacecontrolclient.cpp
                                                  ../space_control/methodhandler.cpp)

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(i3c_client ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(i3c_client ${CONFIG_LIBRARY})

# if wiredPi has been included
if (I2C_ENDPOINT_IMPL=="wiredPi")
  # add link targets for the wiredPi libraries
  # WIREDPI_LIBRARY has already been checked before
  find_library(WIREDPI_DEV_LIBRARY wiredPiDev)
  target_link_libraries(i3c_client ${WIREDPI_LIBRARY} ${WIREDPI_DEV_LIBRARY})
endif (I2C_ENDPOINT_IMPL=="wiredPi")
