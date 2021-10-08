# frozen_string_literal: true

module KKM
  module Models
    # A utility class for printing text lines
    MarkingCode = Struct.new(:type, :code, :status, :quantity, :measure_unit, :mode, :fractional_quantity)
  end
end
