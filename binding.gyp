{
    "targets": [
        {
            "target_name": "node_sourcekit",
            "sources": [
                "cpp/node-sourcekit.cpp",
                "cpp/SourceKitWorker.cpp"
            ],
            "include_dirs": [
                "cpp",
                "<!(node -e \"require('nan')\")"
            ],
            "cflags": [
                "-Wall",
                "-std=c++11"
            ],
            "conditions": [
                [
                    "OS==\"mac\"",
                    {
                        "xcode_settings": {
                            "MACOSX_DEPLOYMENT_TARGET": "10.7",
                            "OTHER_CPLUSPLUSFLAGS": [
                                "-std=c++11",
                                "-stdlib=libc++",
                                "-F$(HOME)/Source/build/Ninja-RelWithDebInfoAssert/swift-macosx-x86_64/lib"
                            ],
                            "OTHER_LDFLAGS": [
                                "-stdlib=libc++",
                                "-F$(HOME)/Source/build/Ninja-RelWithDebInfoAssert/swift-macosx-x86_64/lib",
                                "-framework sourcekitd",
                                "-Wl,-rpath",
                                "-Wl,$(HOME)/Source/build/Ninja-RelWithDebInfoAssert/swift-macosx-x86_64/lib"
                            ]
                        }
                    }
                ]
            ]
        }
    ]
}
