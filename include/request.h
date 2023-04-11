#ifndef WORMHOLE_REQUEST_H_
#define WORMHOLE_REQUEST_H_

#include <string_view>

namespace wormhole {

struct Response {
  char* data;
  size_t size;
  long code;
};

Response request(const std::string_view url);

} // namespace wormhole

#endif // WORMHOLE_REQUEST_H_
