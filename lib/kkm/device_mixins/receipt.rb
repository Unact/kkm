# frozen_string_literal: true

module Kkm
  module DeviceMixins
    # A utility mixin for printing receipt
    module Receipt
      # rubocop:disable Metrics/CyclomaticComplexity
      def print_receipt(receipt_data, electronically: true)
        raise TypeError, "Parameter must be a Models::Receipt" unless receipt_data.is_a?(Models::Receipt)

        setup_operator(receipt_data.operator)
        setup_tags(receipt_data.tags)
        set_param(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE, receipt_data.type)
        set_param(LibFptr::LIBFPTR_PARAM_RECEIPT_ELECTRONICALLY, electronically)
        open_receipt

        receipt_data.positions.each { |position| register_position(position) }
        register_receipt_total(receipt_data.total) unless receipt_data.total.nil?
        receipt_data.payments.each { |payment| register_payment(payment) }
        receipt_data.taxes.each { |tax| register_receipt_tax(tax) }

        close_receipt
      rescue DeviceError => e
        data = Kkm::Models::Data.new(LibFptr::LIBFPTR_DT_RECEIPT_STATE)

        cancel_receipt if e.code != LibFptr::LIBFPTR_ERROR_CONNECTION_DISABLED &&
                          e.code != LibFptr::LIBFPTR_ERROR_CONNECTION_LOST &&
                          retrieve_data(data)[:receipt_type] != LibFptr::LIBFPTR_RT_CLOSED

        raise e
      end
      # rubocop:enable Metrics/CyclomaticComplexity

      private

      def register_position(data)
        raise TypeError, "Parameter must be a Models::Receipt::Position" unless data.is_a?(Models::Receipt::Position)

        setup_tags(data.tags)
        set_param(LibFptr::LIBFPTR_PARAM_COMMODITY_NAME, data.name)
        set_param(LibFptr::LIBFPTR_PARAM_PRICE, data.price)
        set_param(LibFptr::LIBFPTR_PARAM_QUANTITY, data.quantity)
        set_param(LibFptr::LIBFPTR_PARAM_TAX_TYPE, data.tax_type)

        registration
      end

      def register_payment(data)
        raise TypeError, "Parameter must be a Models::Receipt::Payment" unless data.is_a?(Models::Receipt::Payment)

        set_param(LibFptr::LIBFPTR_PARAM_PAYMENT_SUM, data.sum)
        set_param(LibFptr::LIBFPTR_PARAM_PAYMENT_TYPE, data.type)
        payment
      end

      def register_receipt_total(total)
        set_param(LibFptr::LIBFPTR_PARAM_SUM, total)
        receipt_total
      end

      def register_receipt_tax(data)
        raise TypeError, "Parameter must be a Models::Receipt::Tax" unless data.is_a?(Models::Receipt::Tax)

        set_param(LibFptr::LIBFPTR_PARAM_TAX_SUM, data.sum)
        set_param(LibFptr::LIBFPTR_PARAM_TAX_TYPE, data.type)
        receipt_tax
      end
    end
  end
end
