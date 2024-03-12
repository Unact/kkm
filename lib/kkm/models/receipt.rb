# frozen_string_literal: true

module KKM
  module Models
    # A utility class for printing a receipt
    Receipt = Struct.new(:type, :positions, :payments, :tags, :taxes, :total, :operator) do
      # rubocop:disable Metrics/ParameterLists
      def initialize(type = nil, positions = [], payments = [], tags = [], taxes = [], total = nil, operator = nil)
        super(type, positions, payments, tags, taxes, total, operator)
      end
      # rubocop:enable Metrics/ParameterLists
    end

    Receipt::Position = Struct.new(:name, :quantity, :price, :tax_type, :tags, :marking)
    Receipt::Marking = Struct.new(:code, :status, :mode, :result, :id)
    Receipt::Payment = Struct.new(:sum, :type)
    Receipt::Tax = Struct.new(:sum, :type)
    Receipt::Total = Struct.new(:sum, :type)
  end
end
