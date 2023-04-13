#ifndef WORMHOLE_REQUEST_H_
#define WORMHOLE_REQUEST_H_

#include <filesystem>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace wormhole {

#define WORMHOLE_HTTP_VERSIONS(V)                                              \
  V(1_0)                                                                       \
  V(1_1)                                                                       \
  V(2_0)                                                                       \
  V(2TLS)                                                                      \
  V(2_PRIOR_KNOWLEDGE)                                                         \
  V(3)                                                                         \
  V(3ONLY)

enum class HTTPVersion {
#define V(HTTP_VERSION) v##HTTP_VERSION,

  WORMHOLE_HTTP_VERSIONS(V)
#undef V
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
  std::optional<std::filesystem::path> ca_bundle() const { return ca_bundle_; }

private:
  RequestOptions(HTTPVersion http_version, Method method,
                 std::optional<std::filesystem::path> ca_bundle)
      : http_version_{http_version}, method_{method}, ca_bundle_{std::move(
                                                          ca_bundle)} {}

  friend RequestOptionsBuilder;

  HTTPVersion http_version_;
  Method method_;
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

  RequestOptionsBuilder &set_ca_bundle(std::filesystem::path ca_bundle) {
    ca_bundle_ = std::move(ca_bundle);
    return *this;
  }

  RequestOptions build() {
    return RequestOptions(http_version_, method_, std::move(ca_bundle_));
  }

private:
  HTTPVersion http_version_ = HTTPVersion::v2TLS;
  Method method_ = Method::GET;
  std::optional<std::filesystem::path> ca_bundle_;
};

struct Response {
  std::ostringstream body;
  long code;
  std::optional<std::string> error;
};

Response request(const std::string_view url,
                 RequestOptions options = RequestOptionsBuilder().build());

} // namespace wormhole

#endif // WORMHOLE_REQUEST_H_
