#ifndef WORMHOLE_REQUEST_H_
#define WORMHOLE_REQUEST_H_

#include <optional>
#include <string>
#include <string_view>

namespace wormhole {

struct Response {
  char *data;
  size_t size;
  long code;
  std::optional<std::string> error;
};

Response request(const std::string_view url);

} // namespace wormhole

#endif // WORMHOLE_REQUEST_H_
