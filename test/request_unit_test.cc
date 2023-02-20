#include <request.h>

#include <gtest/gtest.h>

#include <string>

TEST(http, http_basic) {
  wormhole::Response res = wormhole::request("https://postman-echo.com/get");
  std::string response_string(res.data, res.size);
  ASSERT_NE(response_string.find(R"("url": "https://postman-echo.com/get")"), std::string::npos);
}
