require 'support/spec_helper'

RSpec.describe Kkm::DeviceInterface do
  def setup_test_data
    @test_device_interface = Kkm::DeviceInterface.new YAML.load_file(File.open('./spec/device_settings/kkm.yaml'))
    @test_goods = [
      {name: 'Test Goods 1', quantity: 10, price: 12, tax: :'10'},
      {name: 'Test Goods 2', quantity: 20, price: 20, tax: :'18'}
    ]
    @test_fiscal_props = [{
      number: Kkm::Constants::FiscalProperty::CASHIER,
      value: "Тестовый",
      type: Kkm::Constants::FiscalPropertyType::STRING
    }]
    @test_payments = [{
      sum: 270,
      type: Kkm::Constants::PaymentType::CASH
    }, {
      sum: 250,
      type: Kkm::Constants::PaymentType::CASHLESS
    }]
  end

  before(:all) do
    setup_test_data
    @test_device_interface.turn_on
  end

  after(:all) do
    @test_device_interface.turn_off
  end

  before(:each) do
    @test_device_interface.new_document
  end

  it 'beep should be heard' do
    expect{@test_device_interface.beep}.to_not raise_error(Kkm::DeviceDriverError)
  end

  it 'should print text' do
    expect{@test_device_interface.print_text("Test text")}.to_not raise_error(Kkm::DeviceDriverError)
  end

  it 'should set correct mode' do
    @test_device_interface.setup_mode Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR
    expect(@test_device_interface.get_mode).to eq(Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR)
  end

  context 'goods cheque' do
    after(:each) do
      @test_device_interface.print_report Kkm::Constants::ReportType::REPORT_Z
    end

    it 'should print correct cheque' do
      expect{@test_device_interface.print_cheque_sell(@test_goods, @test_payments)}.to_not raise_error(Kkm::DeviceDriverError)
      expect(@test_device_interface.get_change).to eq(0)
      expect(@test_device_interface.get_remainder).to eq(0)
      expect(@test_device_interface.get_mode).to eq(Kkm::Constants::Mode::MODE_REGISTRATION)
      expect(@test_device_interface.get_check_type).to eq(Kkm::Constants::ChequeType::CHEQUE_SELL)
    end

    it 'should return correct check info' do
      @test_device_interface.print_cheque_sell(@test_goods, @test_payments, @test_fiscal_props)
      last_check_info = @test_device_interface.last_check_info

      expect(last_check_info[:sum]).to eq(@test_payments.reduce(0){|acc, pay| acc + pay[:sum]})
      expect(last_check_info[:datetime].to_date).to eq(Date.today)
    end

    it 'should return correct sums for different payments' do
      @test_device_interface.print_cheque_sell(@test_goods, @test_payments, @test_fiscal_props)

      expect(@test_device_interface.payment_register(Kkm::Constants::PaymentType::CASH)).to eq(@test_payments.first[:sum])
      expect(@test_device_interface.payment_register(Kkm::Constants::PaymentType::CASHLESS)).to eq(@test_payments.last[:sum])
    end
  end

  context '#print_report' do
    it 'should print correct report' do
      @test_device_interface.print_report Kkm::Constants::ReportType::REPORT_Z

      expect(@test_device_interface.get_report_type).to eq(Kkm::Constants::ReportType::REPORT_Z)
    end

    it 'should use correct mode' do
      @test_device_interface.print_report Kkm::Constants::ReportType::REPORT_X, Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR

      expect(@test_device_interface.get_mode).to eq(Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR)
      expect(@test_device_interface.get_report_type).to eq(Kkm::Constants::ReportType::REPORT_X)
    end
  end
end
