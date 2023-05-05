# frozen_string_literal: true

module KKM
  module Models
    # A utility class for printing a receipt
    class Receipt
      Position = Struct.new(:name, :quantity, :price, :tax_type, :tags, :marking)
      Marking = Struct.new(:code, :status, :mode, :result, :id)
      Payment = Struct.new(:sum, :type)
      Tax = Struct.new(:sum, :type)
      Total = Struct.new(:sum, :type)

      attr_accessor :type, :positions, :payments, :tags, :taxes, :total, :operator

      def initialize
        @type = nil
        @positions = []
        @payments = []
        @tags = []
        @taxes = []
        @total = nil
        @operator = nil
      end
    end
  end
end
