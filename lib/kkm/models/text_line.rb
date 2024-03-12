# frozen_string_literal: true

module KKM
  module Models
    # A utility class for printing text lines
    TextLine = Struct.new(:text, :alignment, :wrap) do
      def initialize(text = "", alignment = LibFptr::LIBFPTR_ALIGNMENT_CENTER, wrap = LibFptr::LIBFPTR_TW_WORDS)
        super(text, alignment, wrap)
      end
    end
  end
end
