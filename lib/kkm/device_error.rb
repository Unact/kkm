# frozen_string_literal: true

module KKM
  # Error for all device failures
  class DeviceError < StandardError
    attr_reader :code, :error_msg, :device_name

    def initialize(code, error_msg, device_name)
      @code = code
      @error_msg = error_msg
      @device_name = device_name
    end

    def to_s
      "#{@device_name} - #{@code} [#{@error_msg}]"
    end
  end
end
