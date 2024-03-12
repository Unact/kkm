# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility mixin for retrieving fn data
    module FNData
      # rubocop:disable Metrics/CyclomaticComplexity
      def retrieve_fn_data(fn_data)
        raise TypeError, "Parameter must be a Models::FNData" unless fn_data.is_a?(Models::FNData)

        case fn_data.type
        when LibFptr::LIBFPTR_FNDT_DOCUMENT_BY_NUMBER then fn_document_by_number_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_LAST_RECEIPT then fn_last_receipt_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_LAST_DOCUMENT then fn_last_document_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_FN_INFO then fn_info_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_OFD_EXCHANGE_STATUS then fn_ofd_exchange_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_REG_INFO then fn_reg_info_data(fn_data)
        when LibFptr::LIBFPTR_FNDT_SHIFT then fn_shift_data(fn_data)
        else
          raise "Unknown fn data type"
        end
      end
      # rubocop:enable Metrics/CyclomaticComplexity

      private

      def query_with_fn_data(fn_data)
        set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, fn_data.type)
        set_param(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER, fn_data.document_number) if fn_data.document_number
        fn_query_data
      end

      # rubocop:disable Metrics/CyclomaticComplexity
      def fn_document_by_number_data(fn_data)
        query_with_fn_data(fn_data)

        result = {
          type: get_param_int(LibFptr::LIBFPTR_PARAM_FN_DOCUMENT_TYPE),
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
          has_ofd_ticket: get_param_bool(LibFptr::LIBFPTR_PARAM_HAS_OFD_TICKET),
          fiscal_sign: get_param_str(LibFptr::LIBFPTR_PARAM_FISCAL_SIGN)
        }

        case result[:type]
        when LibFptr::LIBFPTR_FN_DOC_REGISTRATION
          result.merge(
            organization_vatin: get_param_str(Constants::Tag::ORGANIZATION_VATIN),
            registration_number: get_param_str(Constants::Tag::REGISTRATION_NUMBER),
            taxation_types: get_param_int(Constants::Tag::TAXATION_TYPES),
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
            usage_conditions: get_param_int(Constants::Tag::USAGE_CONDITIONS),
            ofd_vatin: get_param_str(Constants::Tag::OFD_VATIN)
          )
        when LibFptr::LIBFPTR_FN_DOC_OPEN_SHIFT, LibFptr::LIBFPTR_FN_DOC_CLOSE_SHIFT
          result.merge(shift_number: get_param_int(Constants::Tag::SHIFT_NUMBER))
        when
          LibFptr::LIBFPTR_FN_DOC_RECEIPT,
          LibFptr::LIBFPTR_FN_DOC_BSO,
          LibFptr::LIBFPTR_FN_DOC_CORRECTION,
          LibFptr::LIBFPTR_FN_DOC_BSO_CORRECTION
        then
          if get_param_bool(LibFptr::LIBFPTR_PARAM_HAS_ADDITIONAL_DATA)
            result.merge(
              receipt_type: get_param_int(Constants::Tag::RECEIPT_TYPE),
              sum: get_param_double(Constants::Tag::SUM),
              receipt_cash_payment_type: get_param_double(Constants::Tag::RECEIPT_CASH_PAYMENT_TYPE),
              receipt_electronically_payment_type:
                get_param_double(Constants::Tag::RECEIPT_ELECTRONICALLY_PAYMENT_TYPE),
              receipt_prepaid_payment_type: get_param_double(Constants::Tag::RECEIPT_PREPAID_PAYMENT_TYPE),
              receipt_credit_payment_type: get_param_double(Constants::Tag::RECEIPT_CREDIT_PAYMENT_TYPE),
              receipt_other_payment_type: get_param_double(Constants::Tag::RECEIPT_OTHER_PAYMENT_TYPE),
              vat20_sum: get_param_double(Constants::Tag::VAT20_SUM),
              vat10_sum: get_param_double(Constants::Tag::VAT10_SUM),
              vat0_sum: get_param_double(Constants::Tag::VAT0_SUM),
              no_sum: get_param_double(Constants::Tag::NO_SUM),
              vat120_sum: get_param_double(Constants::Tag::VAT120_SUM),
              vat110_sum: get_param_double(Constants::Tag::VAT110_SUM),
              additional_receipt_param: get_param_str(Constants::Tag::ADDITIONAL_RECEIPT_PARAM),
              fiscal_sign_archive: get_param_bytearray(LibFptr::LIBFPTR_PARAM_FISCAL_SIGN_ARCHIVE)
            )
          else
            result.merge(
              receipt_type: get_param_int(Constants::Tag::RECEIPT_TYPE),
              sum: get_param_double(Constants::Tag::SUM)
            )
          end
        when LibFptr::LIBFPTR_FN_DOC_CLOSE_FN
          result.merge(
            organization_vatin: get_param_str(Constants::Tag::ORGANIZATION_VATIN),
            registration_number: get_param_str(Constants::Tag::REGISTRATION_NUMBER)
          )
        when LibFptr::LIBFPTR_FN_DOC_OPERATOR_CONFIRMATION
          result
        when LibFptr::LIBFPTR_FN_DOC_REREGISTRATION
          result.merge(
            organization_vatin: get_param_str(Constants::Tag::ORGANIZATION_VATIN),
            registration_number: get_param_str(Constants::Tag::REGISTRATION_NUMBER),
            taxation_types: get_param_int(Constants::Tag::TAXATION_TYPES),
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
            usage_conditions: get_param_int(Constants::Tag::USAGE_CONDITIONS),
            ofd_vatin: get_param_str(Constants::Tag::OFD_VATIN),
            reregistration_codes: get_param_int(Constants::Tag::REREGISTRATION_CODES)
          )
        when LibFptr::LIBFPTR_FN_DOC_EXCHANGE_STATUS
          result.merge(
            not_sent_fd_total: get_param_int(Constants::Tag::NOT_SENT_FD_TOTAL),
            first_not_sent_fd_datetime: get_param_datetime(Constants::Tag::FIRST_NOT_SENT_FD_DATETIME)
          )
        else
          raise "Unknown type"
        end
      end
      # rubocop:enable Metrics/CyclomaticComplexity

      def fn_last_receipt_data(fn_data)
        query_with_fn_data(fn_data)

        {
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
          receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
          fiscal_sign: get_param_str(LibFptr::LIBFPTR_PARAM_FISCAL_SIGN),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER)
        }
      end

      def fn_last_document_data(fn_data)
        query_with_fn_data(fn_data)

        {
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          fiscal_sign: get_param_str(LibFptr::LIBFPTR_PARAM_FISCAL_SIGN),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER)
        }
      end

      def fn_info_data(fn_data)
        query_with_fn_data(fn_data)

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
        query_with_fn_data(fn_data)

        {
          ofd_exchange_status: get_param_int(LibFptr::LIBFPTR_PARAM_OFD_EXCHANGE_STATUS),
          documents_count: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENTS_COUNT),
          document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
          datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
          ofd_message_read: get_param_bool(LibFptr::LIBFPTR_PARAM_OFD_MESSAGE_READ)
        }
      end

      def fn_reg_info_data(fn_data)
        query_with_fn_data(fn_data)

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
        query_with_fn_data(fn_data)

        {
          shift_number: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_NUMBER),
          receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER)
        }
      end
    end
  end
end
