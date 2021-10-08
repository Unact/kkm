# frozen_string_literal: true

module KKM
  module Models
    # A utility class for passing settings
    # raw_settings is a hash containing all KKM::LibFptr::LIBFPTR_SETTING_ + the following keys
    # "Timeout" - timeout for device connection and marking check
    # "DeviceId" - unique identificator for your device
    # "DeviceName" - human readable name for your device
    Settings = Struct.new(:raw_settings)
  end
end
