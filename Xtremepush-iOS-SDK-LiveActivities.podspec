Pod::Spec.new do |s|
  s.name = "Xtremepush-iOS-SDK-LiveActivities"
  s.version = "6.1.2"
  s.summary = "Xtremepush Live Activities helper module"
  s.license = { "type" => "Commercial", "file" => "LICENSE" }
  s.authors = { "Xtremepush" => "info@xtremepush.com" }
  s.homepage = "https://github.com/xtremepush/Xtremepush-iOS-SDK"
  s.description = "Swift-only helper APIs for Live Activities token management."
  s.source = { :git => "https://github.com/xtremepush/Xtremepush-iOS-SDK.git", :tag => "6.1.2" }

  s.ios.deployment_target = "16.2"
  s.swift_version = "5.7"

  s.module_name = "XPushLiveActivities"
  s.source_files = "Sources/XPushLiveActivities/**/*.swift"

  s.dependency "Xtremepush-iOS-SDK", "6.1.2"
end