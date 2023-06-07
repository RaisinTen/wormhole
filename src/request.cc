#include <request.h>

#include <curl/curl.h>

#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>

#include <iostream>

namespace wormhole {

namespace {

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  *static_cast<std::ostringstream *>(userdata)
      << std::string{const_cast<const char *>(ptr), size * nmemb};
  return size * nmemb;
}

auto set_curl_http_version(CURL *curl, HTTPVersion version) -> void {
  long curl_http_version;

  switch (version) {
  case HTTPVersion::v1_0:
    curl_http_version = CURL_HTTP_VERSION_1_0;
    break;
  case HTTPVersion::v1_1:
    curl_http_version = CURL_HTTP_VERSION_1_1;
    break;
  case HTTPVersion::v2:
    curl_http_version = CURL_HTTP_VERSION_2;
    break;
  case HTTPVersion::v2_TLS:
    curl_http_version = CURL_HTTP_VERSION_2TLS;
    break;
  case HTTPVersion::v2_PRIOR_KNOWLEDGE:
    curl_http_version = CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE;
    break;
  case HTTPVersion::v3:
    curl_http_version = CURL_HTTP_VERSION_3;
    break;
  case HTTPVersion::v3_ONLY:
    curl_http_version = CURL_HTTP_VERSION_3ONLY;
    break;
  }

  curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, curl_http_version);
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

    set_curl_http_version(curl, options.http_version());

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

    if (options.body().has_value()) {
      std::string body{options.body().value()};
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
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

/* ----------------------------- WebSocket ------------------------------ */

std::optional<std::string> WebSocket::send(const std::string &message) {
  size_t sent;
  CURLcode res = curl_ws_send(curl_, message.data(), message.length(), &sent, 0,
                              CURLWS_TEXT);

  if (res != CURLE_OK) {
    return curl_easy_strerror(res);
  }

  return std::nullopt;
}

std::optional<std::string> WebSocket::disconnect() {
  size_t sent;
  CURLcode res = curl_ws_send(curl_, "", 0, &sent, 0, CURLWS_CLOSE);

  if (res != CURLE_OK) {
    return curl_easy_strerror(res);
  }

  return std::nullopt;
}

namespace {

struct WebSocketWriteCallbackData {
  WebSocket *web_socket;
  std::function<void(WebSocket *, const std::string &)> write_callback;
};

size_t web_socket_write_callback(char *ptr, size_t size, size_t nmemb,
                                 void *userdata) {
  WebSocketWriteCallbackData *web_socket_write_callback_data =
      static_cast<WebSocketWriteCallbackData *>(userdata);
  web_socket_write_callback_data->write_callback(
      web_socket_write_callback_data->web_socket,
      std::string{const_cast<const char *>(ptr), size * nmemb});
  return size * nmemb;
}

} // namespace

void request(
    const std::string_view url,
    std::function<void(WebSocket *, const std::string &)> write_callback) {
  CURL *curl;

  curl = curl_easy_init();

  WebSocket web_socket(curl);
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, web_socket_write_callback);
    WebSocketWriteCallbackData write_callback_data{&web_socket, write_callback};
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_callback_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      web_socket.error_ = curl_easy_strerror(res);
    }

    curl_easy_cleanup(curl);
  }
}

} // namespace wormhole
