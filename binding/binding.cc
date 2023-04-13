#include <napi.h>

#include <request.h>

#include <filesystem>
#include <optional>
#include <string>

class HttpRequestWorker : public Napi::AsyncWorker {
public:
  HttpRequestWorker(Napi::Env env, std::string url,
                    wormhole::RequestOptions options)
      : Napi::AsyncWorker(env), url_(std::move(url)),
        options_(std::move(options)),
        deferred_(Napi::Promise::Deferred::New(env)) {}

  void Execute() {
    response_ = wormhole::request(url_, options_);
    if (response_->error.has_value()) {
      SetError(response_->error.value());
    }
  }

  void OnOK() {
    Napi::Object response_object = Napi::Object::New(Env());
    response_object.Set("body", response_->body.str());
    response_object.Set("code", response_->code);
    deferred_.Resolve(response_object);
  }

  void OnError(Napi::Error const &error) { deferred_.Reject(error.Value()); }

  Napi::Promise GetPromise() const { return deferred_.Promise(); }

private:
  std::string url_;
  wormhole::RequestOptions options_;
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

  wormhole::RequestOptionsBuilder request_options_builder;

  if (info.Length() >= 2) {
    if (!info[1].IsObject()) {
      Napi::TypeError::New(env, "The second argument needs to be an object.")
          .ThrowAsJavaScriptException();
      return {};
    }

    Napi::Object options(env, info[1]);

    if (options.Has("http")) {
      Napi::Value value = options.Get("http");
      if (!value.IsString()) {
        Napi::TypeError::New(env, "The http version needs to be a string.")
            .ThrowAsJavaScriptException();
        return {};
      }

      std::string http_version_string(Napi::String(env, value));

      if (false) {
#define V(HTTP_VERSION)                                                        \
  }                                                                            \
  else if (http_version_string == "v" #HTTP_VERSION) {                         \
    request_options_builder.set_http_version(                                  \
        wormhole::HTTPVersion::v##HTTP_VERSION);

        WORMHOLE_HTTP_VERSIONS(V)
#undef V
      } else {
        Napi::TypeError::New(env, "Invalid http version string.")
            .ThrowAsJavaScriptException();
        return {};
      }
    }

    if (options.Has("method")) {
      Napi::Value value = options.Get("method");
      if (!value.IsString()) {
        Napi::TypeError::New(env, "The method needs to be a string.")
            .ThrowAsJavaScriptException();
        return {};
      }

      std::string method_string(Napi::String(env, value));

      if (false) {
#define V(HTTP_METHOD)                                                         \
  }                                                                            \
  else if (method_string == #HTTP_METHOD) {                                    \
    request_options_builder.set_method(wormhole::Method::HTTP_METHOD);

        WORMHOLE_HTTP_METHODS(V)
#undef V
      } else {
        Napi::TypeError::New(env, "Invalid method string.")
            .ThrowAsJavaScriptException();
        return {};
      }
    }

    if (options.Has("ca")) {
      Napi::Value value = options.Get("ca");
      if (!value.IsString()) {
        Napi::TypeError::New(env, "The ca needs to be a string.")
            .ThrowAsJavaScriptException();
        return {};
      }

      std::string ca_string(Napi::String(env, value));
      std::filesystem::path ca_path{ca_string};
      std::error_code error_code;
      if (!std::filesystem::exists(ca_path, error_code)) {
        Napi::TypeError::New(env, "Invalid ca bundle.")
            .ThrowAsJavaScriptException();
        return {};
      }
      request_options_builder.set_ca_bundle(std::move(ca_path));
    }
  }

  HttpRequestWorker *worker =
      new HttpRequestWorker(env, url, request_options_builder.build());
  auto promise = worker->GetPromise();
  worker->Queue();
  return promise;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("request", Napi::Function::New(env, ::Request));
  return exports;
}

NODE_API_MODULE(testaddon, InitAll)
