require 'support/spec_helper'

RSpec.describe Kkm::Device do
  before :all do
    device_settings = YAML.load_file(File.open('./spec/device_settings/kkm.yaml'))
    @test_device = Kkm::Device.new device_settings
  end

  it 'should print text' do
    expect {
      @test_device.work do |driver|
        driver.print_text "Test text"
      end
    }.to_not raise_error(RuntimeError)
  end
end
