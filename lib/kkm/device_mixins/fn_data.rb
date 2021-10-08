# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility mixin for retrieving fn data
    module FNData
      def retrieve_fn_data(fn_data)
        raise TypeError, "Parameter must be a Models::FNData" unless fn_data.is_a?(Models::FNData)

        case fn_data.type
        when LibFptr::LIBFPTR_FNDT_LAST_RECEIPT then fn_last_receipt_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_FN_INFO then fn_info_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_OFD_EXCHANGE_STATUS then fn_ofd_exchange_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_REG_INFO then fn_registration_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_SHIFT then fn_shift_data(fn_data)
        else
          raise "Unknown fn data type"
        end
      end

      private

      def fn_last_receipt_data(fn_data)
        set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, fn_data.type)
        fn_query_data

        {
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
          receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
          fiscal_sign: get_param_str(LibFptr::LIBFPTR_PARAM_FISCAL_SIGN),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER)
        }
      end

      def fn_info_data(fn_data)
        set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, fn_data.type)
        fn_query_data

        {
          serial_number: get_param_str(LibFptr::LIBFPTR_PARAM_SERIAL_NUMBER),
          version: get_param_str(LibFptr::LIBFPTR_PARAM_FN_VERSION),
          type: get_param_int(LibFptr::LIBFPTR_PARAM_FN_TYPE),
          state: get_param_int(LibFptr::LIBFPTR_PARAM_FN_STATE),
          flags: get_param_int(LibFptr::LIBFPTR_PARAM_FN_FLAGS),
          need_replacement: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_NEED_REPLACEMENT),
          resource_exhausted: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_RESOURCE_EXHAUSTED),
          memory_overflow: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_MEMORY_OVERFLOW),
          ofd_timeout: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_OFD_TIMEOUT),
          critical_error: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_CRITICAL_ERROR)
        }
      end

      def fn_ofd_exchange_data(fn_data)
        set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, fn_data.type)
        fn_query_data

        {
          ofd_exchange_status: get_param_int(LibFptr::LIBFPTR_PARAM_OFD_EXCHANGE_STATUS),
          documents_count: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENTS_COUNT),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          ofd_message_read: get_param_bool(LibFptr::LIBFPTR_PARAM_OFD_MESSAGE_READ)
        }
      end

      def fn_registration_data(fn_data)
        set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, fn_data.type)
        fn_query_data

        {
          taxation_types: get_param_int(Constants::Tag::TAXATION_TYPES),
          agent_type: get_param_int(Constants::Tag::AGENT_TYPE),
          ffd_version: get_param_int(Constants::Tag::FFD_VERSION),
          auto_mode: get_param_bool(Constants::Tag::AUTO_MODE),
          offline_mode: get_param_bool(Constants::Tag::OFFLINE_MODE),
          encryption: get_param_bool(Constants::Tag::ENCRYPTION),
          internet: get_param_bool(Constants::Tag::INTERNET),
          service: get_param_bool(Constants::Tag::SERVICE),
          bso: get_param_bool(Constants::Tag::BSO),
          lottery: get_param_bool(Constants::Tag::LOTTERY),
          gambling: get_param_bool(Constants::Tag::GAMBLING),
          excise: get_param_bool(Constants::Tag::EXCISE),
          machine_installation: get_param_bool(Constants::Tag::MACHINE_INSTALLATION),
          organization_vatin: get_param_str(Constants::Tag::ORGANIZATION_VATIN),
          organization_name: get_param_str(Constants::Tag::ORGANIZATION_NAME),
          organization_email: get_param_str(Constants::Tag::ORGANIZATION_EMAIL),
          payments_address: get_param_str(Constants::Tag::PAYMENTS_ADDRESS),
          payments_place: get_param_str(Constants::Tag::PAYMENTS_PLACE),
          registration_number: get_param_str(Constants::Tag::REGISTRATION_NUMBER),
          machine_number: get_param_str(Constants::Tag::MACHINE_NUMBER),
          ofd_vatin: get_param_str(Constants::Tag::OFD_VATIN),
          ofd_name: get_param_str(Constants::Tag::OFD_NAME)
        }
      end

      def fn_shift_data(fn_data)
        set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, fn_data.type)
        fn_query_data

        {
          shift_number: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_NUMBER),
          receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER)
        }
      end
    end
  end
end
