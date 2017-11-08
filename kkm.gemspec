$:.push File.expand_path("../lib", __FILE__)

require "kkm/version"

Gem::Specification.new do |s|
  s.name        = "kkm"
  s.version     = Kkm::VERSION
  s.authors     = ["Unact"]
  s.homepage    = "https://github.com/Unact/kkm"
  s.summary     = "Интерфейс связи с ККМ"
  s.description = ""
  s.license     = "MIT"

  s.files = Dir["lib/**/*", "ext/**/*.{cpp,rb}", "MIT-LICENSE", "Rakefile", "README.rdoc"]
  s.test_files = Dir["spec/**/*"]

  s.add_dependency "nokogiri"

  s.add_development_dependency "rake-compiler"
  s.add_development_dependency "rspec"
  s.add_development_dependency "byebug"

  s.extensions = %w[ext/kkm/extconf.rb]
end
