              Pod::Spec.new do |s|
                s.name = "Xtremepush-iOS-SDK"
                s.version = "0.1.4"
                s.summary = "This is the Xtremepush iOS SDK for Mobile Marketing Automation"
                s.license = { "type" => "Commercial", "file" => "LICENSE"}
                s.authors = { "Xtremepush" => "info@xtremepush.com"}
                s.homepage = "https://github.com/xtremepush/Xtremepush-iOS-SDK"
                s.description = "This is the Xtremepush iOS SDK for Mobile Marketing Automation"
                s.source = { :path => '.' }
                s.ios.deployment_target    = '8.0'
                s.ios.vendored_framework   = 'XPush.framework'
              end