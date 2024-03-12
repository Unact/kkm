# frozen_string_literal: true

module KKM
  module Models
    # A utility class for passing fiscal tags
    Tag = Struct.new(:number, :value, :print, :tags) do
      # rubocop:disable Style/OptionalBooleanParameter
      # rubocop:disable Metrics/ParameterLists
      def initialize(number = nil, value = nil, print = true, tags = [])
        super(number, value, print, tags)
      end
      # rubocop:enable Style/OptionalBooleanParameter
      # rubocop:enable Metrics/ParameterLists
    end
  end
end
