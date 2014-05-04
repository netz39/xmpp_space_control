project(i3c_client)

cmake_minimum_required(VERSION 2.8)


add_executable(i3c_client main.cpp                ../space_control/configuredclientfactory.cpp
                                                  ../space_control/spacecontrolclient.cpp
                                                  ../space_control/methodhandler.cpp)

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(i3c_client ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(i3c_client ${CONFIG_LIBRARY})
