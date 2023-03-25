add_executable(request_unit_test "${GTEST_MAIN}" request_unit_test.cc)
target_include_directories(request_unit_test PRIVATE ../include)
target_link_libraries(request_unit_test PRIVATE gtest)
target_link_libraries(request_unit_test PRIVATE wormhole)
# This is not using:
# target_link_directories(request_unit_test PRIVATE ${CURL_CONFIG_PREFIX_PATH}/relocatable_lib)
# because that unnecessarily adds an rpath to the link directory.
target_link_libraries(request_unit_test PRIVATE "-L${CURL_CONFIG_PREFIX_PATH}/relocatable_lib")
target_link_libraries(request_unit_test PRIVATE curl)
target_link_libraries(request_unit_test PRIVATE "-rpath @executable_path/../../curl_build/vendor_install")
add_googletest(request request_unit_test)
