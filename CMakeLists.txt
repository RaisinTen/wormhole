cmake_minimum_required(VERSION 3.21)
project(wormhole VERSION 1.0.0 DESCRIPTION "A custom networking layer for Postman")

set(CMAKE_CXX_STANDARD 20)

add_library(wormhole STATIC
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

enable_testing()
include("${PROJECT_SOURCE_DIR}/cmake/googletest.cmake")
add_subdirectory(vendor/googletest)
add_subdirectory(test)

# Checks
file(GLOB_RECURSE WORMHOLE_CXX_SOURCE_FILES
  src/*.cc src/*.h
  test/*.cc test/*.h
  binding/*.cc)
include("${PROJECT_SOURCE_DIR}/cmake/clang-format.cmake")
