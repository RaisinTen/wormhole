#include <request.h>

#include <curl/curl.h>

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
    fprintf(stderr, "Failed to allocate memory.\n");
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
    fprintf(stderr, "Failed to allocate memory.\n");
    return data;
  }

  data.data[0] = '\0';

  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
  }
  return data;
}

} // namespace wormhole
