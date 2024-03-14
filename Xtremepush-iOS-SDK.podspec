Pod::Spec.new do |s|
    s.name = "Xtremepush-iOS-SDK"
    s.version = "5.7.10"
    s.summary = "Xtremepush iOS SDK for Mobile Marketing Automation"
    s.license = { "type" => "Commercial", "file" => "LICENSE"}
    s.authors = { "Xtremepush" => "info@xtremepush.com"}
    s.homepage = "https://github.com/xtremepush/Xtremepush-iOS-SDK"
    s.description = "This is the Xtremepush iOS SDK for Mobile Marketing Automation"
    s.source = { :git => 'https://github.com/xtremepush/Xtremepush-iOS-SDK.git', :tag => '5.7.10' }
    s.ios.deployment_target    = '11.0'
    s.ios.vendored_framework   = 'XPush.xcframework'
end