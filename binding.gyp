{
  "variables": {
    "relocatable_libwormhole_dir": "relocatable_libwormhole_<!(uname -m)",
  },
  "targets": [{
    "target_name": "wormhole",
    "cflags!": [ "-fno-exceptions" ],
    "cflags_cc!": [ "-fno-exceptions" ],
    "sources": [
      "binding/binding.cc",
    ],
    'include_dirs': [
      "<!@(node -p \"require('node-addon-api').include\")",
      "<(relocatable_libwormhole_dir)/include"
    ],
    'libraries': [
      "-L<(module_root_dir)/<(relocatable_libwormhole_dir)/lib -lwormhole",
      "-rpath @loader_path/../../<(relocatable_libwormhole_dir)"
    ],
    'dependencies': [
      "<!(node -p \"require('node-addon-api').gyp\")"
    ],
    'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
  }]
}
