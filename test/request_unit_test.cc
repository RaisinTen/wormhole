#include "../include/request.h"

#include <string>

int main() {
  wormhole::Response res = wormhole::request("https://postman-echo.com/get");
  std::string response_string(res.data, res.size);

  if (response_string.find(R"("url": "https://postman-echo.com/get")") == std::string::npos) {
    exit(1);
  }

  return 0;
}
