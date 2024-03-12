# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility mixin for retrieving kkm data
    module Data
      def retrieve_data(data)
        raise TypeError, "Parameter must be a Models::Data" unless data.is_a?(Models::Data)

        case data.type
        when LibFptr::LIBFPTR_DT_STATUS then status_data(data)
        when LibFptr::LIBFPTR_DT_DATE_TIME then datetime_data(data)
        when LibFptr::LIBFPTR_DT_PAYMENT_SUM then payment_sum_data(data)
        when LibFptr::LIBFPTR_DT_SHIFT_STATE then shift_state_data(data)
        when LibFptr::LIBFPTR_DT_RECEIPT_STATE then receipt_state_data(data)
        else
          raise "Unknown data type"
        end
      end

      private

      def query_with_data(data)
        set_param(LibFptr::LIBFPTR_PARAM_DATA_TYPE, data.type)
        set_param(LibFptr::LIBFPTR_PARAM_PAYMENT_TYPE, data.payment_type) if data.payment_type
        set_param(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE, data.receipt_type) if data.receipt_type
        query_data
      end

      def status_data(data)
        query_with_data(data)

        {
          operator_id: get_param_int(LibFptr::LIBFPTR_PARAM_OPERATOR_ID),
          logical_number: get_param_int(LibFptr::LIBFPTR_PARAM_LOGICAL_NUMBER),
          shift_state: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_STATE),
          model: get_param_int(LibFptr::LIBFPTR_PARAM_MODEL),
          mode: get_param_int(LibFptr::LIBFPTR_PARAM_MODE),
          submode: get_param_int(LibFptr::LIBFPTR_PARAM_SUBMODE),
          receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
          shift_number: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_NUMBER),
          receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
          receipt_line_length: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH),
          receipt_line_length_pix: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH_PIX),
          receipt_sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          fiscal: get_param_bool(LibFptr::LIBFPTR_PARAM_FISCAL),
          fn_fiscal: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_FISCAL),
          fn_present: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_PRESENT),
          invalid_fn: get_param_bool(LibFptr::LIBFPTR_PARAM_INVALID_FN),
          cashdrawer_opened: get_param_bool(LibFptr::LIBFPTR_PARAM_CASHDRAWER_OPENED),
          paper_present: get_param_bool(LibFptr::LIBFPTR_PARAM_RECEIPT_PAPER_PRESENT),
          paper_near_end: get_param_bool(LibFptr::LIBFPTR_PARAM_PAPER_NEAR_END),
          cover_opened: get_param_bool(LibFptr::LIBFPTR_PARAM_COVER_OPENED),
          printer_connection_lost: get_param_bool(LibFptr::LIBFPTR_PARAM_PRINTER_CONNECTION_LOST),
          printer_error: get_param_bool(LibFptr::LIBFPTR_PARAM_PRINTER_ERROR),
          cut_error: get_param_bool(LibFptr::LIBFPTR_PARAM_CUT_ERROR),
          printer_overheat: get_param_bool(LibFptr::LIBFPTR_PARAM_PRINTER_OVERHEAT),
          blocked: get_param_bool(LibFptr::LIBFPTR_PARAM_BLOCKED),
          serial_number: get_param_str(LibFptr::LIBFPTR_PARAM_SERIAL_NUMBER),
          model_name: get_param_str(LibFptr::LIBFPTR_PARAM_MODEL_NAME),
          unit_version: get_param_str(LibFptr::LIBFPTR_PARAM_UNIT_VERSION)
        }
      end

      def datetime_data(data)
        query_with_data(data)

        {
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME)
        }
      end

      def payment_sum_data(data)
        query_with_data(data)

        {
          sum: get_param_double(LibFptr::LIBFPTR_PARAM_SUM)
        }
      end

      def shift_state_data(data)
        query_with_data(data)

        {
          state: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_STATE),
          shift_number: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_NUMBER),
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME)
        }
      end

      def receipt_state_data(data)
        query_with_data(data)

        {
          receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
          receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER),
          document_number: get_param_str(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
          receipt_sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
          remainder: get_param_double(LibFptr::LIBFPTR_PARAM_REMAINDER),
          change: get_param_double(LibFptr::LIBFPTR_PARAM_CHANGE)
        }
      end
    end
  end
end
