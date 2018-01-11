Pod::Spec.new do |s|
  s.name = "XtremePush"
  s.version = "0.1.3"
  s.summary = "Hello, this is a test"
  s.license = {"type"=>"MIT", "file"=>"LICENSE"}
  s.authors = {"volkzayaz"=>"volkzayaz@gmail.com"}
  s.homepage = "https://github.com/volkzayaz/XPush"
  s.description = "TODO: Add long description of the pod here."
  s.source = { :path => '.' }
  s.ios.deployment_target    = '7.0'
  s.ios.vendored_framework   = 'XPush.framework'
end