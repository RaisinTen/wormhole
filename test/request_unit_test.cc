#include <request.h>

#include <gtest/gtest.h>

#include <string>

TEST(http, http_basic) {
  wormhole::Response res = wormhole::request("https://postman-echo.com/get");
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  std::string response_string(res.data, res.size);
  ASSERT_NE(response_string.find(R"("url": "https://postman-echo.com/get")"),
            std::string::npos);
}

TEST(http, http_quic) {
  // Found this url in https://bagder.github.io/HTTP3-test/.
  wormhole::Response res = wormhole::request("https://quic.aiortc.org/");
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  std::string response_string(res.data, res.size);
  ASSERT_NE(response_string.find(
                "Congratulations, you loaded this page using HTTP/3"),
            std::string::npos);
}

TEST(http, http_404_not_found) {
  wormhole::Response res =
      wormhole::request("https://example.com/404-not-found");
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 404);
}

TEST(http, illegal_url_format) {
  wormhole::Response res = wormhole::request("abc");
  ASSERT_EQ(res.error.has_value(), true);
  ASSERT_EQ(res.error.value(), "URL using bad/illegal format or missing URL");
}

TEST(http, post_request) {
  wormhole::Response res = wormhole::request(
      "https://postman-echo.com/post", wormhole::RequestOptionsBuilder()
                                           .set_method(wormhole::Method::POST)
                                           .build());
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  std::string response_string(res.data, res.size);
  ASSERT_NE(response_string.find(R"("url": "https://postman-echo.com/post")"),
            std::string::npos);
}
