# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility class for printing nonfiscal data on receipt
    module Text
      def print_text_line(text_line_data)
        raise TypeError, "Parameter must be a Models::TextLine" unless text_line_data.is_a?(Models::TextLine)

        set_param(LibFptr::LIBFPTR_PARAM_TEXT, text_line_data.text)
        set_param(LibFptr::LIBFPTR_PARAM_ALIGNMENT, text_line_data.alignment)
        set_param(LibFptr::LIBFPTR_PARAM_TEXT_WRAP, text_line_data.wrap)

        print_text
      end

      def print_slip
        begin_nonfiscal_document

        yield(self)
        (0..Constants::CHEQUE_CUT_LINES).each { print_text_line(Models::TextLine.new) }
      ensure
        end_nonfiscal_document
      end
    end
  end
end
