{
  'variables': {
    "curl_config_bin%": "<(module_root_dir)/curl_build/vendor_install/bin/curl-config",
  },
  "targets": [{
    "target_name": "wormhole",
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "sources": [
      "binding/binding.cc",
      "include/request.h",
      "src/request.cc",
    ],
    'include_dirs': [
      "<!@(node -p \"require('node-addon-api').include\")",
      "include",
      "<!(<(curl_config_bin) --prefix)/include"
    ],
    'libraries': [
      "-L<!(<(curl_config_bin) --prefix)/relocatable_lib -lcurl",
      "-rpath @loader_path/../../curl_build/vendor_install",
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
  }]
}
