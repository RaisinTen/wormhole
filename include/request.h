#ifndef WORMHOLE_REQUEST_H_
#define WORMHOLE_REQUEST_H_

#include <optional>
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

private:
  RequestOptions(Method method) : method_{method} {}

  friend RequestOptionsBuilder;

  Method method_;
};

class RequestOptionsBuilder {
public:
  RequestOptionsBuilder &set_method(Method method) {
    method_ = method;
    return *this;
  }

  RequestOptions build() { return RequestOptions(method_); }

private:
  Method method_ = Method::GET;
};

struct Response {
  char *data;
  size_t size;
  long code;
  std::optional<std::string> error;
};

Response request(const std::string_view url,
                 RequestOptions options = RequestOptionsBuilder().build());

} // namespace wormhole

#endif // WORMHOLE_REQUEST_H_
