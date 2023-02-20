add_test([=[http.http_basic]=]  /Users/raisinten/Desktop/git/wormhole/cmake/test/request_unit_test [==[--gtest_filter=http.http_basic]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[http.http_basic]=]  PROPERTIES WORKING_DIRECTORY /Users/raisinten/Desktop/git/wormhole/cmake/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  request_unit_test_TESTS http.http_basic)
