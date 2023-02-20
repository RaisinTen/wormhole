{
  "targets": [{
    "target_name": "wormhole",
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "sources": [
      "binding/binding.cc",
      "include/request.h",
    ],
    'include_dirs': [
      "<!@(node -p \"require('node-addon-api').include\")",
      "include",
    ],
    'libraries': [
      "build/libwormhole.a",
      "-lcurl",
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
  }]
}
