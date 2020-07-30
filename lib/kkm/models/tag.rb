# frozen_string_literal: true

module KKM
  module Models
    # A utility class for passing fiscal tags
    class Tag
      attr_reader :number, :value, :print, :tags

      def initialize(number = nil, value = nil, print: true, tags: [])
        @number = number
        @value = value
        @print = print
        @tags = tags
      end
    end
  end
end
