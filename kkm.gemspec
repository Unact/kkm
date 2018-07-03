$:.push File.expand_path("../lib", __FILE__)

require "kkm/version"

Gem::Specification.new do |s|
  s.name        = "kkm"
  s.version     = Kkm::VERSION
  s.authors     = ["Unact"]
  s.homepage    = "https://github.com/Unact/kkm"
  s.summary     = "Интерфейс связи с ККМ"
  s.description = "Обертка на ruby для связи с ККМ Атол"
  s.license     = "MIT"
  s.files = Dir["lib/**/*", "ext/**/*", "LICENSE", "Gemfile", "README.md"]
  s.test_files = Dir["spec/**/*"]

  s.add_dependency "nokogiri", '~> 1.3'

  s.add_development_dependency "rake-compiler", '~> 1'
  s.add_development_dependency "rspec", '~> 3'
  s.add_development_dependency "byebug", '~> 9'

  s.extensions = %w(ext/kkm/extconf.rb)
end
