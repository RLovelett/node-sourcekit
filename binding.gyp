{
    "targets": [
        {
            "target_name": "node_sourcekit",
            "sources": [
                "src/node-sourcekit.cpp",
                "src/SourceKitWorker.cpp"
            ],
            "include_dirs": [
                "include",
                "<!(node -e \"require('nan')\")"
            ],
            "cflags": [
                "-Wall",
                "-std=c++11"
            ],
            "conditions": [
                [ "OS==\"mac\"", {
                    "xcode_settings": {
                        "MACOSX_DEPLOYMENT_TARGET": "10.7",
                        "OTHER_CPLUSPLUSFLAGS": [
                            "-std=c++11",
                            "-stdlib=libc++",
                            "-F<!(xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/lib"
                        ],
                        "OTHER_LDFLAGS": [
                            "-stdlib=libc++",
                            "-F<!(xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/lib",
                            "-framework sourcekitd",
                            "-Wl,-rpath",
                            "-Wl,<!(xcode-select -p)/Toolchains/XcodeDefault.xctoolchain/usr/lib"
                        ]
                    }
                }]
            ]
        }
    ]
}
