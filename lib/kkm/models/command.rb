# frozen_string_literal: true

module KKM
  module Models
    # A utility class for passing command data
    Command = Struct.new(:command, :wait, :timeout) do
      # rubocop:disable Style/OptionalBooleanParameter
      def initialize(command = nil, wait = true, timeout = Constants::DEFAULT_TIMEOUT_ENQ)
        super(command, wait, timeout)
      end
      # rubocop:enable Style/OptionalBooleanParameter

      def command_bytearray
        command.split.map { |part| Integer(part, 16) }
      end
    end
  end
end
