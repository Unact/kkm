# frozen_string_literal: true

module KKM
  module Models
    # A utility class for printing text lines
    MarkingCode = Struct.new(
      :type,
      :code,
      :status,
      :mode,
      :measure_unit,
      :quantity,
      :fractional_quantity,
      :timeout,
      :send_to_server
    )
  end
end
