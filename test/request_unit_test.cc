#include <request.h>

#include <gtest/gtest.h>

#include <string>

TEST(http, https_basic) {
  wormhole::Response res = wormhole::request("https://postman-echo.com/get");
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  ASSERT_NE(res.body.find(R"("url": "https://postman-echo.com/get")"),
            std::string::npos);
  ASSERT_GE(std::stoi(res.headers["content-length"]), 250);
}

TEST(http, http3_basic) {
  // Found this url in https://bagder.github.io/HTTP3-test/.
  wormhole::Response res =
      wormhole::request("https://quic.aiortc.org/",
                        wormhole::RequestOptionsBuilder()
                            .set_http_version(wormhole::HTTPVersion::v3_ONLY)
                            .build());
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  ASSERT_NE(res.body.find("Congratulations, you loaded this page using HTTP/3"),
            std::string::npos);
}

TEST(http, 404_not_found) {
  wormhole::Response res =
      wormhole::request("http://example.com/404-not-found");
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 404);
}

TEST(http, illegal_url_format) {
  wormhole::Response res = wormhole::request("abc");
  ASSERT_EQ(res.error.has_value(), true);
  ASSERT_EQ(res.error.value(), "Couldn't resolve host name");
}

TEST(http, post_request) {
  wormhole::Response res = wormhole::request(
      "https://postman-echo.com/post",
      wormhole::RequestOptionsBuilder()
          .set_method(wormhole::Method::POST)
          .set_body("Wormhole body")
          .set_headers({{"hello", "world"}, {"a", "b"}, {"x", "y"}})
          .build());

  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  ASSERT_NE(res.body.find(R"("url": "https://postman-echo.com/post")"),
            std::string::npos);
  ASSERT_NE(res.body.find(R"("hello": "world")"), std::string::npos);
  ASSERT_NE(res.body.find(R"("a": "b")"), std::string::npos);
  ASSERT_NE(res.body.find(R"("x": "y")"), std::string::npos);
  ASSERT_NE(res.body.find(R"("Wormhole body": "")"), std::string::npos);
}

TEST(http, gbk_encoding) {
  // Third most popular website from
  // https://w3techs.com/technologies/details/en-gbk.
  wormhole::Response res = wormhole::request("https://www.69shu.com");
  ASSERT_EQ(res.error.has_value(), false);
  ASSERT_EQ(res.code, 200);
  // TODO(RaisinTen): Test the entire title string:
  // `<title>69书吧_更新最快_无弹窗广告_无错小说阅读网</title>`.
  // Currently, encoding isn't handled correctly but at least this doesn't
  // crash or throw.
  ASSERT_NE(res.body.find("<title>69"), std::string::npos);
}

TEST(http, invalid_ca_bundle) {
  wormhole::Response res = wormhole::request(
      "https://postman-echo.com/post",
      wormhole::RequestOptionsBuilder().set_ca_bundle("/a/b/c").build());
  ASSERT_EQ(res.error, "Invalid CA bundle.");
}

TEST(http, default_request_options) {
  wormhole::RequestOptions options = wormhole::RequestOptionsBuilder().build();
  ASSERT_EQ(options.http_version(), wormhole::HTTPVersion::v2_TLS);
  ASSERT_EQ(options.method(), wormhole::Method::GET);
}
