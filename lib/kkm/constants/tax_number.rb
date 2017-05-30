module Kkm::Constants::TaxNumber
  TAX_0       = 1   # НДС 0%
  TAX_10      = 2   # НДС 10%
  TAX_18      = 3   # НДС 18%
  TAX_NONE    = 4   # БЕЗ НДС
  TAX_10_100  = 5   # НДС 10/100
  TAX_18_118  = 6   # НДС 18/118

  def self.tax_number_by_tax tax
    case tax
    when 0
      TAX_0
    when 10
      TAX_10
    when 18
      TAX_18
    when 110
      TAX_10_100
    when 118
      TAX_18_118
    else
      TAX_NONE
    end
  end
end
