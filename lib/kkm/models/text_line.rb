# frozen_string_literal: true

module Kkm
  module Models
    # A utility class for printing text lines
    class TextLine
      attr_reader :text, :alignment, :wrap

      def initialize(text = "", alignment: LibFptr::LIBFPTR_ALIGNMENT_CENTER, wrap: LibFptr::LIBFPTR_TW_WORDS)
        @text = text
        @alignment = alignment
        @wrap = wrap
      end
    end
  end
end
