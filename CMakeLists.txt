cmake_minimum_required(VERSION 3.21)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15")

project(wormhole VERSION 1.0.0 DESCRIPTION "A custom networking layer for Postman")

set(CMAKE_CXX_STANDARD 20)

set(RELOCATABLE_LIBCURL_DIR "relocatable_libcurl_${CMAKE_SYSTEM_PROCESSOR}")

add_library(wormhole SHARED
  src/request.cc
  )

set_target_properties(wormhole PROPERTIES PUBLIC_HEADER include/request.h)

target_include_directories(wormhole PRIVATE include)
target_include_directories(wormhole PRIVATE src)
target_include_directories(wormhole PRIVATE "${RELOCATABLE_LIBCURL_DIR}/include")

# This is not using:
# target_link_directories(wormhole PRIVATE ../relocatable_libcurl/lib)
# because that unnecessarily adds an rpath to the link directory.
target_link_libraries(wormhole PRIVATE "-L../${RELOCATABLE_LIBCURL_DIR}/lib")
target_link_libraries(wormhole PRIVATE curl)
target_link_libraries(wormhole PRIVATE "-rpath @loader_path/../${RELOCATABLE_LIBCURL_DIR}")

include(GNUInstallDirs)

install(TARGETS wormhole
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY
  "${PROJECT_SOURCE_DIR}/${RELOCATABLE_LIBCURL_DIR}/lib"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/..")

enable_testing()
include("${PROJECT_SOURCE_DIR}/cmake/googletest.cmake")
add_subdirectory(vendor/googletest)
add_subdirectory(test)
