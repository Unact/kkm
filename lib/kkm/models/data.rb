# frozen_string_literal: true

module KKM
  module Models
    # A utility class for passing data for kkm data retrieval
    Data = Struct.new(:type, :receipt_type, :payment_type)
  end
end
