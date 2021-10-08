# frozen_string_literal: true

module KKM
  # Base class
  class KKMError < StandardError; end

  # Error class to use for timeout errors
  class TimeoutError < KKMError; end

  # Error for all device failures
  class DeviceError < KKMError
    attr_reader :code, :error_msg, :device_name

    def initialize(code, error_msg, device_name)
      super()
      @code = code
      @error_msg = error_msg
      @device_name = device_name
    end

    def to_s
      "#{@device_name} - #{@code} [#{@error_msg}]"
    end
  end
end
