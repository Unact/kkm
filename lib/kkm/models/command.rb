# frozen_string_literal: true

module KKM
  module Models
    # A utility class for passing command data
    class Command
      attr_accessor :command, :wait, :timeout

      def initialize(command, wait = true, timeout = Constants::DEFAULT_TIMEOUT_ENQ)
        @command = command
        @wait = wait
        @timeout = timeout
      end

      def command_bytearray
        @command.split.map { |part| Integer(part, 16) }
      end
    end
  end
end
