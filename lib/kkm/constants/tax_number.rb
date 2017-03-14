module Kkm::Constants::TaxNumber
  TAX_0    = 1   # НДС 0%
  TAX_10   = 2   # НДС 10%
  TAX_18   = 3   # НДС 18%
  TAX_NONE = 4   # БЕЗ НДС

  def self.tax_number_by_tax tax
    case tax
    when 0
      TAX_0
    when 10
      TAX_10
    when 18
      TAX_18
    else
      TAX_NONE
    end
  end
end
