set(BUILD_GMOCK OFF)
set(INSTALL_GTEST OFF)
include(GoogleTest)
set(GTEST_MAIN "${PROJECT_SOURCE_DIR}/vendor/googletest/googletest/src/gtest_main.cc")

macro(add_googletest name target)
  # For some very strange reason, gtest_discover_tests()
  # does not seem to work when using the Xcode generator.
  if(CMAKE_GENERATOR STREQUAL "Xcode")
    add_test(NAME ${name} COMMAND "$<TARGET_FILE:${target}>")
  else()
    gtest_discover_tests(${target})
  endif()
endmacro()
