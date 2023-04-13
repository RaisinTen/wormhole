#include <request.h>

#include <curl/curl.h>

#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace wormhole {

namespace {

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  static_cast<Response *>(userdata)->body
      << std::string{const_cast<const char *>(ptr), size * nmemb};
  return size * nmemb;
}

} // namespace

Response request(const std::string_view url, RequestOptions options) {
  CURL *curl;

  Response data;

  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    switch (options.http_version()) {
#define V(HTTP_VERSION)                                                        \
  case HTTPVersion::v##HTTP_VERSION:                                           \
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION,                               \
                     CURL_HTTP_VERSION_##HTTP_VERSION);                        \
    break;

      WORMHOLE_HTTP_VERSIONS(V)
#undef V
    }

    switch (options.method()) {
#define V(HTTP_METHOD)                                                         \
  case Method::HTTP_METHOD:                                                    \
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, #HTTP_METHOD);               \
    break;

      WORMHOLE_HTTP_METHODS(V)
#undef V
    }

    if (options.ca_bundle().has_value()) {
      curl_easy_setopt(curl, CURLOPT_CAINFO,
                       options.ca_bundle().value().c_str());
    }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      data.error = curl_easy_strerror(res);
    } else {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &data.code);
    }

    curl_easy_cleanup(curl);
  }
  return data;
}

} // namespace wormhole
