// swift-tools-version: 5.7
import PackageDescription

let package = Package(
    name: "Xtremepush-iOS-SDK",
    platforms: [
        .iOS(.v12)
    ],
    products: [
        .library(
            name: "XPush",
            targets: ["XPush"]
        ),

        .library(
            name: "XPushLiveActivities",
            targets: ["XPushLiveActivities"]
        ),

        .library(
            name: "XPushFull",
            targets: ["XPushFull"]
        )
    ],
    targets: [
        .binaryTarget(
            name: "XPush",
            path: "XPush.xcframework"
        ),

        .target(
            name: "XPushLiveActivities",
            dependencies: ["XPush"],
            path: "Sources/XPushLiveActivities"
        ),

        .target(
            name: "XPushFull",
            dependencies: ["XPush", "XPushLiveActivities"],
            path: "Sources/XPushFull"
        )
    ]
)

