#include <napi.h>

#include <request.h>

#include <string>

Napi::String Request(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "The first argument needs to be a string.")
        .ThrowAsJavaScriptException();
    return {};
  }

  const std::string url = info[0].As<Napi::String>().Utf8Value();

  wormhole::Response res = wormhole::request(url);

  std::string response_string(res.data, res.size);
  Napi::String returnValue = Napi::String::New(env, response_string);

  return returnValue;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("request", Napi::Function::New(env, ::Request));
  return exports;
}

NODE_API_MODULE(testaddon, InitAll)
