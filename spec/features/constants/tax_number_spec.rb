require 'support/spec_helper'

RSpec.describe Kkm::Constants::TaxNumber do
  it 'should find correct tax' do
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax('0')).to eq(Kkm::Constants::TaxNumber::TAX_0)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax(:'0')).to eq(Kkm::Constants::TaxNumber::TAX_0)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax('10')).to eq(Kkm::Constants::TaxNumber::TAX_10)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax(:'10')).to eq(Kkm::Constants::TaxNumber::TAX_10)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax('18')).to eq(Kkm::Constants::TaxNumber::TAX_18)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax(:'18')).to eq(Kkm::Constants::TaxNumber::TAX_18)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax('10/110')).to eq(Kkm::Constants::TaxNumber::TAX_10_110)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax(:'10/110')).to eq(Kkm::Constants::TaxNumber::TAX_10_110)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax('18/118')).to eq(Kkm::Constants::TaxNumber::TAX_18_118)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax(:'18/118')).to eq(Kkm::Constants::TaxNumber::TAX_18_118)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax('')).to eq(Kkm::Constants::TaxNumber::TAX_NONE)
    expect(Kkm::Constants::TaxNumber.tax_number_by_tax(:'')).to eq(Kkm::Constants::TaxNumber::TAX_NONE)
  end
end
