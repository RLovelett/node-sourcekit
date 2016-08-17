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
                [ "OS==\"linux\"", {
                    "libraries": [
                        "-lsourcekitdInProc",
                        "-L/usr/lib/swift/linux",
                        # TODO: Figure out why --no-as-needed is necessary
                        # Not necessary with Clang, e.g., CC=clang CXX=clang++ LINKXX=clang++ LINK=clang node-gyp rebuild --verbose
                        # Not GCC (6.1.1 20160802) on Arch Linux
                        "-Wl,--no-as-needed",
                        "-lswiftCore",
                        "-lswiftSwiftOnoneSupport",
                        "-Wl,-rpath",
                        "-Wl,/usr/lib/swift/linux"
                    ]
                }],
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
