cmake_minimum_required(VERSION 3.21)

project(wormhole VERSION 1.0.0 DESCRIPTION "A custom networking layer for Postman")

add_library(libwormhole SHARED
  src/request.cc
  )

set_target_properties(libwormhole PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libwormhole PROPERTIES SOVERSION 1)

set_target_properties(libwormhole PROPERTIES PUBLIC_HEADER include/request.h)

target_include_directories(libwormhole PRIVATE include)
target_include_directories(libwormhole PRIVATE src)

include(GNUInstallDirs)

install(TARGETS libwormhole
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
