{
    "targets": [{
        "target_name": "phash",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "cflags": [ "-fexceptions" ],
        "cflags_cc": [ "-fexceptions" ],
        "sources": [
            "src/main.cpp"
        ],
        "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        "libraries": [ "-lpHash" ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_CPP_EXCEPTIONS' ]
    }]
}
