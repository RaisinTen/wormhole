#include <napi.h>

#include <request.h>

#include <optional>
#include <string>

class HttpRequestWorker : public Napi::AsyncWorker {
public:
  HttpRequestWorker(Napi::Env env, std::string url)
      : Napi::AsyncWorker(env), url_(std::move(url)),
        deferred_(Napi::Promise::Deferred::New(env)) {}

  void Execute() { response_ = wormhole::request(url_); }

  void OnOK() {
    Napi::Object response_object = Napi::Object::New(Env());
    std::string response_string(response_->data, response_->size);
    response_object.Set("body", response_string);
    response_object.Set("code", response_->code);
    deferred_.Resolve(response_object);
  }

  void OnError(Napi::Error const &error) { deferred_.Reject(error.Value()); }

  Napi::Promise GetPromise() const { return deferred_.Promise(); }

private:
  std::string url_;
  Napi::Promise::Deferred deferred_;
  std::optional<wormhole::Response> response_;
};

Napi::Value Request(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "The first argument needs to be a string.")
        .ThrowAsJavaScriptException();
    return {};
  }

  const std::string url = info[0].As<Napi::String>().Utf8Value();

  HttpRequestWorker *worker = new HttpRequestWorker(env, url);
  auto promise = worker->GetPromise();
  worker->Queue();
  return promise;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("request", Napi::Function::New(env, ::Request));
  return exports;
}

NODE_API_MODULE(testaddon, InitAll)
