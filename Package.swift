// swift-tools-version: 5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "Xtremepush-iOS-SDK",
    platforms: [ .iOS(.v8) ],
    products: [
        .library(name: "XPush",
                 targets: ["XPush"])
    ],
    targets: [
        .binaryTarget(name: "XPush",
                      path: "XPush.xcframework")
    ]
)
