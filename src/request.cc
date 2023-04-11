#include <request.h>

#include <curl/curl.h>

#include <optional>
#include <string>
#include <string_view>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {

size_t write_data(void *ptr, size_t size, size_t nmemb,
                  wormhole::Response *data) {
  size_t index = data->size;
  size_t n = (size * nmemb);
  char *tmp;

  data->size += (size * nmemb);

#ifdef DEBUG
  fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
  tmp = (char *)realloc(data->data, data->size + 1); /* +1 for '\0' */

  if (tmp) {
    data->data = tmp;
  } else {
    if (data->data) {
      free(data->data);
    }
    data->error = "Failed to allocate memory.";
    return 0;
  }

  memcpy((data->data + index), ptr, n);
  data->data[data->size] = '\0';

  return size * nmemb;
}

} // namespace

namespace wormhole {

Response request(const std::string_view url) {
  CURL *curl;

  Response data;
  data.size = 0;
  data.data = (char *)malloc(4096); /* reasonable size initial buffer */
  if (NULL == data.data) {
    data.error = "Failed to allocate memory.";
    return data;
  }

  data.data[0] = '\0';

  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // CURL_HTTP_VERSION_3: (Added in 7.66.0) This option makes libcurl attempt
    // to use HTTP/3 to the host given in the URL, with fallback to earlier HTTP
    // versions if needed.
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_3);

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
