#ifndef WORMHOLE_REQUEST_H_
#define WORMHOLE_REQUEST_H_

#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace wormhole {

enum class HTTPVersion {
  v1_0,
  v1_1,
  v2,
  v2_TLS,
  v2_PRIOR_KNOWLEDGE,
  v3,
  v3_ONLY
};

#define WORMHOLE_HTTP_METHODS(V)                                               \
  V(GET)                                                                       \
  V(HEAD)                                                                      \
  V(POST) V(PUT) V(DELETE) V(CONNECT) V(OPTIONS) V(TRACE) V(PATCH)

enum class Method {
#define V(HTTP_METHOD) HTTP_METHOD,

  WORMHOLE_HTTP_METHODS(V)
#undef V
};

class RequestOptionsBuilder;

class RequestOptions {
public:
  HTTPVersion http_version() const { return http_version_; }
  Method method() const { return method_; }
  const std::map<std::string, std::string> &headers() const { return headers_; }
  std::optional<std::filesystem::path> ca_bundle() const { return ca_bundle_; }

private:
  RequestOptions(HTTPVersion http_version, Method method,
                 std::map<std::string, std::string> headers,
                 std::optional<std::filesystem::path> ca_bundle)
      : http_version_{http_version}, method_{method},
        headers_{std::move(headers)}, ca_bundle_{std::move(ca_bundle)} {}

  friend RequestOptionsBuilder;

  HTTPVersion http_version_;
  Method method_;
  std::map<std::string, std::string> headers_;
  std::optional<std::filesystem::path> ca_bundle_;
};

class RequestOptionsBuilder {
public:
  RequestOptionsBuilder &set_http_version(HTTPVersion http_version) {
    http_version_ = http_version;
    return *this;
  }

  RequestOptionsBuilder &set_method(Method method) {
    method_ = method;
    return *this;
  }

  RequestOptionsBuilder &
  set_headers(std::map<std::string, std::string> headers) {
    headers_ = std::move(headers);
    return *this;
  }

  RequestOptionsBuilder &set_ca_bundle(std::filesystem::path ca_bundle) {
    ca_bundle_ = std::move(ca_bundle);
    return *this;
  }

  RequestOptions build() {
    return RequestOptions(http_version_, method_, std::move(headers_),
                          std::move(ca_bundle_));
  }

private:
  HTTPVersion http_version_ = HTTPVersion::v2_TLS;
  Method method_ = Method::GET;
  std::map<std::string, std::string> headers_;
  std::optional<std::filesystem::path> ca_bundle_;
};

struct Response {
  std::string body;
  long code;
  std::map<std::string, std::string> headers;
  std::optional<std::string> error;
};

Response request(const std::string_view url,
                 RequestOptions options = RequestOptionsBuilder().build());

} // namespace wormhole

#endif // WORMHOLE_REQUEST_H_
