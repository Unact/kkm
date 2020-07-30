# frozen_string_literal: true

module Kkm
  module Models
    # A utility class for passing data for report printing
    Report = Struct.new(:type, :operator, :document_number)
  end
end
