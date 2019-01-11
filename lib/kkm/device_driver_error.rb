module Kkm
  class DeviceDriverError < StandardError
    attr_reader :code, :error_msg, :device_name

    def initialize code, error_msg, driver
      @code = code
      @error_msg = error_msg
      @device_name = driver.get_device_single_setting('DeviceName')
    end

    def to_s
      "#{@device_name} - #{@error_msg}"
    end
  end
end
