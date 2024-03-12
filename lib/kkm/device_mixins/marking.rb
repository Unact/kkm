# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility class for working with marking data
    module Marking
      # rubocop:disable Metrics/CyclomaticComplexity
      def check_marking_code(marking_code)
        raise TypeError, "Parameter must be a Models::MarkingCode" unless marking_code.is_a?(Models::MarkingCode)

        cancel_marking_code_validation
        set_param(LibFptr::LIBFPTR_PARAM_MARKING_CODE_TYPE, marking_code.type) if marking_code.type
        set_param(LibFptr::LIBFPTR_PARAM_MARKING_CODE, marking_code.code)
        set_param(LibFptr::LIBFPTR_PARAM_MARKING_CODE_STATUS, marking_code.status)
        set_param(LibFptr::LIBFPTR_PARAM_QUANTITY, marking_code.quantity) if marking_code.quantity
        set_param(LibFptr::LIBFPTR_PARAM_MEASUREMENT_UNIT, marking_code.measure_unit) if marking_code.measure_unit
        set_param(LibFptr::LIBFPTR_PARAM_MARKING_PROCESSING_MODE, marking_code.mode)
        if marking_code.fractional_quantity
          set_param(LibFptr::LIBFPTR_PARAM_MARKING_FRACTIONAL_QUANTITY, marking_code.fractional_quantity)
        end
        set_param(LibFptr::LIBFPTR_PARAM_TIMEOUT, marking_code.timeout) if marking_code.timeout
        if marking_code.send_to_server
          set_param(LibFptr::LIBFPTR_PARAM_MARKING_NOT_SEND_TO_SERVER, marking_code.send_to_server)
        end
        begin_marking_code_validation
        wait_for_marking_code_status
        marking_result
      end
      # rubocop:enable Metrics/CyclomaticComplexity

      private

      def wait_for_marking_code_status
        process_time = Process.clock_gettime(Process::CLOCK_MONOTONIC)
        loop do
          get_marking_code_validation_status

          break if get_param_bool(LibFptr::LIBFPTR_PARAM_MARKING_CODE_VALIDATION_READY)

          if Process.clock_gettime(Process::CLOCK_MONOTONIC) >= process_time + (@timeout / 1000.0)
            cancel_marking_code_validation
            raise TimeoutError, "Couldn't validate marking code in #{@timeout} seconds"
          end
        end
      end

      # rubocop:disable Metrics/CyclomaticComplexity
      def marking_result
        tlv_list = get_param_str(LibFptr::LIBFPTR_PARAM_TLV_LIST).split(";").map(&:to_i)

        {
          is_request_sent: get_param_int(LibFptr::LIBFPTR_PARAM_IS_REQUEST_SENT),
          online_validation_result: get_param_int(LibFptr::LIBFPTR_PARAM_MARKING_CODE_ONLINE_VALIDATION_RESULT),
          online_validation_error: get_param_int(LibFptr::LIBFPTR_PARAM_MARKING_CODE_ONLINE_VALIDATION_ERROR),
          online_validation_error_description:
            get_param_int(LibFptr::LIBFPTR_PARAM_MARKING_CODE_ONLINE_VALIDATION_ERROR_DESCRIPTION),
          marking_online_result: tlv_list.include?(Constants::Tag::MARKING_ONLINE_RESULT) ?
            get_param_int(Constants::Tag::MARKING_ONLINE_RESULT) :
            nil,
          marking_code_type_result: tlv_list.include?(Constants::Tag::MARKING_CODE_TYPE_RESULT) ?
            get_param_int(Constants::Tag::MARKING_CODE_TYPE_RESULT) :
            nil,
          marking_code_type: tlv_list.include?(Constants::Tag::MARKING_CODE_TYPE) ?
            get_param_str(Constants::Tag::MARKING_CODE_TYPE) :
            nil,
          marking_processing_mode: tlv_list.include?(Constants::Tag::MARKING_PROCESSING_MODE) ?
            get_param_int(Constants::Tag::MARKING_PROCESSING_MODE) :
            nil,
          marking_online_result_code: tlv_list.include?(Constants::Tag::MARKING_ONLINE_RESULT_CODE) ?
            get_param_int(Constants::Tag::MARKING_ONLINE_RESULT_CODE) :
            nil,
          marking_position_online_result_code: tlv_list.include?(Constants::Tag::MARKING_POSITION_ONLINE_RESULT) ?
            get_param_int(Constants::Tag::MARKING_POSITION_ONLINE_RESULT) :
            nil
        }
      end
      # rubocop:enable Metrics/CyclomaticComplexity
    end
  end
end
