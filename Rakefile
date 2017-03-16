require 'rake/extensiontask'
require 'rspec/core/rake_task'

# Для rake-compiler
Rake::ExtensionTask.new do |ext|
  ext.name = 'kkm'
  ext.lib_dir = 'lib/kkm'
  ext.ext_dir = 'ext/kkm'
  ext.tmp_dir = 'tmp'
  ext.source_pattern = "*.{c,cpp}"
  ext.gem_spec = Gem::Specification.load('kkm.gemspec')
end

# Для rspec
begin
  RSpec::Core::RakeTask.new(:spec)

  task :default => :spec
rescue LoadError
  puts 'You must `gem install rspec` and `bundle install` to run rake tasks'
end
