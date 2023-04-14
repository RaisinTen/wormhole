#include <request.h>

#include <curl/curl.h>

#include <optional>
#include <sstream>
#include <string>
#include <string_view>

namespace wormhole {

namespace {

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  *static_cast<std::ostringstream *>(userdata)
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
    std::ostringstream write_callback_data;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_callback_data);

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

    curl_slist *request_headers_chunk = NULL;
    for (const auto &[key, value] : options.headers()) {
      request_headers_chunk = curl_slist_append(request_headers_chunk,
                                                (key + ": " + value).c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_headers_chunk);

    if (options.ca_bundle().has_value()) {
      std::filesystem::path ca_path{options.ca_bundle().value()};
      std::error_code error_code;
      if (!std::filesystem::exists(ca_path, error_code)) {
        data.error = "Invalid CA bundle.";
        return data;
      }
      curl_easy_setopt(curl, CURLOPT_CAINFO, ca_path.c_str());
    }

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      data.error = curl_easy_strerror(res);
    } else {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &data.code);
    }

    data.body = write_callback_data.str();

    curl_header *current_header;
    curl_header *previous_header = nullptr;
    do {
      current_header =
          curl_easy_nextheader(curl, CURLH_HEADER, -1, previous_header);
      if (current_header) {
        data.headers[current_header->name] = current_header->value;
      }
      previous_header = current_header;
    } while (current_header);

    curl_easy_cleanup(curl);

    curl_slist_free_all(request_headers_chunk);
  }
  return data;
}

} // namespace wormhole
