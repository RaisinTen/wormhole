{
  "targets": [{
    "target_name": "wormhole",
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "sources": [
      "binding/binding.cc",
    ],
    'include_dirs': [
      "<!@(node -p \"require('node-addon-api').include\")",
      "relocatable_libwormhole/include"
    ],
    'libraries': [
      "-L<(module_root_dir)/relocatable_libwormhole/lib -lwormhole",
      "-rpath @loader_path/../../relocatable_libwormhole",
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
  }]
}
