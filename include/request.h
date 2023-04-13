#ifndef WORMHOLE_REQUEST_H_
#define WORMHOLE_REQUEST_H_

#include <filesystem>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace wormhole {

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
  Method method() const { return method_; }
  std::optional<std::filesystem::path> ca_bundle() const { return ca_bundle_; }

private:
  RequestOptions(Method method, std::optional<std::filesystem::path> ca_bundle)
      : method_{method}, ca_bundle_{std::move(ca_bundle)} {}

  friend RequestOptionsBuilder;

  Method method_;
  std::optional<std::filesystem::path> ca_bundle_;
};

class RequestOptionsBuilder {
public:
  RequestOptionsBuilder &set_method(Method method) {
    method_ = method;
    return *this;
  }

  RequestOptionsBuilder &set_ca_bundle(std::filesystem::path ca_bundle) {
    ca_bundle_ = std::move(ca_bundle);
    return *this;
  }

  RequestOptions build() {
    return RequestOptions(method_, std::move(ca_bundle_));
  }

private:
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
