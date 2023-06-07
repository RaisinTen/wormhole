#include <napi.h>

#include <request.h>

#include <filesystem>
#include <optional>
#include <string>

namespace {
auto to_http_version(const std::string &versionString)
    -> std::optional<wormhole::HTTPVersion> {
  if (versionString == "v1.0") {
    return wormhole::HTTPVersion::v1_0;
  } else if (versionString == "1.1") {
    return wormhole::HTTPVersion::v1_1;
  } else if (versionString == "2.0") {
    return wormhole::HTTPVersion::v2;
  } else if (versionString == "2-TLS") {
    return wormhole::HTTPVersion::v2_TLS;
  } else if (versionString == "2-prior-knowledge") {
    return wormhole::HTTPVersion::v2_PRIOR_KNOWLEDGE;
  } else if (versionString == "3") {
    return wormhole::HTTPVersion::v3;
  } else if (versionString == "3-only") {
    return wormhole::HTTPVersion::v3_ONLY;
  } else {
    return std::nullopt;
  }
}
} // namespace

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
    response_object.Set("body", response_->body);
    response_object.Set("code", response_->code);

    Napi::Object headers = Napi::Object::New(Env());
    for (auto const &[key, val] : response_->headers) {
      headers.Set(key, val);
    }
    response_object.Set("headers", headers);

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

class WebSocketRequestWorker : public Napi::AsyncWorker {
public:
  WebSocketRequestWorker(Napi::Env env, std::string url,
                         Napi::Function callback)
      : Napi::AsyncWorker(env), url_(std::move(url)),
        callback_(callback), tsfn_{Napi::ThreadSafeFunction::New(
                                 env, callback, "WebSocketRequestWorker", 0,
                                 1)} {}

  ~WebSocketRequestWorker() { tsfn_.Release(); }

  void Execute() {
    wormhole::request(url_, [this](wormhole::WebSocket *web_socket,
                                   const std::string &incoming_message) {
      napi_status status = tsfn_.BlockingCall([incoming_message, web_socket](
                                                  Napi::Env env,
                                                  Napi::Function js_callback) {
        // TODO(RaisinTen): Find a way to create this object just once and
        // reusing it instead of constructing it every time this function is
        // called.
        Napi::Object handle = Napi::Object::New(env);
        handle["send"] = Napi::Function::New(
            env, [web_socket](const Napi::CallbackInfo &info) {
              Napi::Env env = info.Env();

              if (!info[0].IsString()) {
                Napi::TypeError::New(env,
                                     "The first argument needs to be a string.")
                    .ThrowAsJavaScriptException();
                return;
              }

              const std::string message =
                  info[0].As<Napi::String>().Utf8Value();

              web_socket->send(message);
            });
        handle["disconnect"] = Napi::Function::New(
            env, [web_socket](const Napi::CallbackInfo &info) {
              web_socket->disconnect();
            });
        js_callback.Call({handle, Napi::String::New(env, incoming_message)});
      });
      if (status != napi_ok) {
        SetError("Napi::ThreadSafeNapi::Function.BlockingCall() failed");
        return;
      }
    });
  }

  void OnOK() { ; }

  void OnError(Napi::Error const &error) { ; }

private:
  std::string url_;
  Napi::Function callback_;
  Napi::ThreadSafeFunction tsfn_;
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

    if (options.Has("httpVersion")) {
      Napi::Value value = options.Get("httpVersion");
      if (!value.IsString()) {
        Napi::TypeError::New(env, "The http version needs to be a string.")
            .ThrowAsJavaScriptException();
        return {};
      }

      std::string http_version_string(Napi::String(env, value));
      std::optional<wormhole::HTTPVersion> http_version =
          to_http_version(http_version_string);

      if (!http_version.has_value()) {
        Napi::TypeError::New(env, "Invalid http version string.")
            .ThrowAsJavaScriptException();
        return {};
      }
      request_options_builder.set_http_version(http_version.value());
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

    if (options.Has("headers")) {
      Napi::Value value = options.Get("headers");
      if (!value.IsObject()) {
        Napi::TypeError::New(env, "The headers needs to be an object.")
            .ThrowAsJavaScriptException();
        return {};
      }

      Napi::Object headers_value(env, value);
      Napi::Array header_names = headers_value.GetPropertyNames();

      std::map<std::string, std::string> headers;
      for (uint32_t i = 0; i < header_names.Length(); ++i) {
        Napi::Value key = header_names.Get(i);

        if (!key.IsString()) {
          Napi::TypeError::New(env, "The header key need to be strings.")
              .ThrowAsJavaScriptException();
          return {};
        }

        Napi::Value val = headers_value.Get(key);

        if (!val.IsString()) {
          Napi::TypeError::New(env, "The header values need to be strings.")
              .ThrowAsJavaScriptException();
          return {};
        }

        headers[Napi::String(env, key)] = Napi::String(env, val);
      }

      request_options_builder.set_headers(std::move(headers));
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
      request_options_builder.set_ca_bundle(std::move(ca_path));
    }

    if (options.Has("body")) {
      Napi::Value value = options.Get("body");
      if (!value.IsString()) {
        Napi::TypeError::New(env, "The body needs to be a string.")
            .ThrowAsJavaScriptException();
        return {};
      }

      request_options_builder.set_body(Napi::String(env, value));
    }
  }

  HttpRequestWorker *worker =
      new HttpRequestWorker(env, url, request_options_builder.build());
  auto promise = worker->GetPromise();
  worker->Queue();
  return promise;
}

Napi::Value WebSocketRequest(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "The first argument needs to be a string.")
        .ThrowAsJavaScriptException();
    return {};
  }

  const std::string url = info[0].As<Napi::String>().Utf8Value();

  if (!info[1].IsFunction()) {
    Napi::TypeError::New(env, "The second argument needs to be a function.")
        .ThrowAsJavaScriptException();
    return {};
  }

  Napi::Function callback = info[1].As<Napi::Function>();

  WebSocketRequestWorker *worker =
      new WebSocketRequestWorker(env, url, callback);
  worker->Queue();
  return callback;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  exports.Set("request", Napi::Function::New(env, ::Request));
  exports.Set("websocketRequest", Napi::Function::New(env, ::WebSocketRequest));
  return exports;
}

NODE_API_MODULE(testaddon, InitAll)
