{
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
      "<(module_root_dir)/relocatable/include"
    ],
    'libraries': [
      "-L<(module_root_dir)/native_build -lwormhole",
      "-rpath @loader_path/../../native_build",
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
  }]
}
