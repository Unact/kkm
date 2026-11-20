# frozen_string_literal: true

require File.expand_path("#{__dir__}/lib/kkm/version")

Gem::Specification.new do |s|
  s.name        = "kkm"
  s.version     = KKM::VERSION
  s.email       = "it@unact.ru"
  s.authors     = ["Unact"]
  s.homepage    = "https://github.com/Unact/kkm"
  s.summary     = "Интерфейс для связи и печати чеков на ККТ"
  s.description = "Имплеметация ДТО ККТ Атол"
  s.license     = "MIT"
  s.files       = Dir["lib/**/*", "LICENSE", "Gemfile", "README.md", "CHANGELOG.md"]
  s.metadata    = { "rubygems_mfa_required" => "true" }

  s.add_dependency "ffi"
  s.required_ruby_version = Gem::Requirement.new(">= 3.0.0")
end
