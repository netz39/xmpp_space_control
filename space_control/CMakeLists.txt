project(xmpp_space_control)

cmake_minimum_required(VERSION 2.8)

add_executable(xmpp_space_control main.cpp configuredclientfactory.cpp spacecontrolclient.cpp)

find_library(GLOOX_LIBRARY gloox)
target_link_libraries(xmpp_space_control ${GLOOX_LIBRARY})

find_library(CONFIG_LIBRARY config++)
target_link_libraries(xmpp_space_control ${CONFIG_LIBRARY})
