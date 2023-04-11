#include <napi.h>

#include <request.h>

#include <string>

Napi::Object Request(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "The first argument needs to be a string.")
        .ThrowAsJavaScriptException();
    return {};
  }

  const std::string url = info[0].As<Napi::String>().Utf8Value();

  wormhole::Response res = wormhole::request(url);

  std::string response_string(res.data, res.size);

  Napi::Object returnValue = Napi::Object::New(env);
  returnValue.Set("body", response_string);
  returnValue.Set("code", res.code);

  return returnValue;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("request", Napi::Function::New(env, ::Request));
  return exports;
}

NODE_API_MODULE(testaddon, InitAll)
