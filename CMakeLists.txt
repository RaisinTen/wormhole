cmake_minimum_required(VERSION 3.21)

project(wormhole VERSION 1.0.0 DESCRIPTION "A custom networking layer for Postman")

add_library(wormhole SHARED
  src/request.cc
  )

set_target_properties(wormhole PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(wormhole PROPERTIES SOVERSION 1)

set_target_properties(wormhole PROPERTIES PUBLIC_HEADER include/request.h)

target_include_directories(wormhole PRIVATE include)
target_include_directories(wormhole PRIVATE src)

find_package(CURL REQUIRED)
target_include_directories(wormhole PRIVATE ${CURL_INCLUDE_DIR})
target_link_libraries(wormhole ${CURL_LIBRARIES})

include(GNUInstallDirs)

install(TARGETS wormhole
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
