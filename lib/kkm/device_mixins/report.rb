# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility mixin for printing reports
    module Report
      def print_report(report_data)
        raise TypeError, "Parameter must be a Models::Report" unless report_data.is_a?(Models::Report)

        setup_operator(report_data.operator)

        set_param(LibFptr::LIBFPTR_PARAM_REPORT_TYPE, report_data.type)
        unless report_data.document_number.nil?
          set_param(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER, report_data.document_number)
        end

        report
      end
    end
  end
end
