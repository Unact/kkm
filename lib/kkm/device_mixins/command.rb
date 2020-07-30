# frozen_string_literal: true

module Kkm
  module DeviceMixins
    # A utility mixin for executing kkm commands
    module Command
      def execute_command(command_data)
        raise TypeError, "Parameter must be a Models::Command" unless command_data.is_a?(Models::Command)

        set_param(LibFptr::LIBFPTR_PARAM_COMMAND_BUFFER, command_data.command_bytearray)
        set_param(LibFptr::LIBFPTR_PARAM_TIMEOUT_ENQ, command_data.timeout)
        set_param(LibFptr::LIBFPTR_PARAM_NO_NEED_ANSWER, !command_data.wait)
        run_command

        get_param_bytearray(LibFptr::LIBFPTR_PARAM_ANSWER_BUFFER).map { |v| v.to_s(16).rjust(2, "0").upcase }
      end
    end
  end
end
