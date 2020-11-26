Pod::Spec.new do |s|
    s.name = "Xtremepush-iOS-SDK"
    s.version = "4.1.11"
    s.summary = "Xtremepush iOS SDK for Mobile Marketing Automation"
    s.license = { "type" => "Commercial", "file" => "LICENSE"}
    s.authors = { "Xtremepush" => "info@xtremepush.com"}
    s.homepage = "https://github.com/xtremepush/Xtremepush-iOS-SDK"
    s.description = "This is the Xtremepush iOS SDK for Mobile Marketing Automation"
    s.source = { :git => 'https://github.com/xtremepush/Xtremepush-iOS-SDK.git', :tag => '4.1.11' }
    s.ios.deployment_target    = '8.0'
    s.ios.vendored_framework   = 'XPush.framework'
    s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
    s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
end