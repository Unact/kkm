require 'support/spec_helper'

RSpec.describe Kkm::DeviceInterface do
  let!(:goods) do
    [
      {name: 'Test Goods 1', quantity: 10, price: 12, tax: :'10'},
      {name: 'Test Goods 2', quantity: 20, price: 20, tax: :'18'}
    ]
  end
  let!(:device_interface) { Kkm::DeviceInterface.new YAML.load_file(File.open('./spec/device_settings/kkm.yaml')) }
  let!(:fiscal_props) do
    [{
      number: Kkm::Constants::FiscalProperty::CASHIER,
      value: "Тестовый",
      type: Kkm::Constants::FiscalPropertyType::STRING
    }]
  end
  let!(:payments) do
    [{
      sum: 270,
      type: Kkm::Constants::PaymentType::CASH
    }, {
      sum: 250,
      type: Kkm::Constants::PaymentType::CASHLESS
    }]
  end

  before do
    device_interface.turn_on
    device_interface.new_document
    device_interface.get_session_opened ? device_interface.close_day : nil
  end

  after do
    device_interface.turn_off
  end

  context 'basic' do
    it 'beep should be heard' do
      expect{device_interface.beep}.to_not raise_error(Kkm::DeviceDriverError)
    end

    it 'should print text' do
      expect{device_interface.print_text("Test text")}.to_not raise_error(Kkm::DeviceDriverError)
    end

    it 'should set correct mode' do
      device_interface.setup_mode Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR
      expect(device_interface.get_mode).to eq(Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR)
    end
  end

  context 'goods cheque' do
    after do
      device_interface.print_report Kkm::Constants::ReportType::REPORT_Z
    end

    it 'should print correct cheque' do
      expect{device_interface.print_cheque_sell(goods, payments, fiscal_props)}.to_not raise_error(Kkm::DeviceDriverError)
      expect(device_interface.get_change).to eq(0)
      expect(device_interface.get_remainder).to eq(0)
      expect(device_interface.get_mode).to eq(Kkm::Constants::Mode::MODE_REGISTRATION)
      expect(device_interface.get_check_type).to eq(Kkm::Constants::ChequeType::CHEQUE_SELL)
    end

    it 'should return correct check info' do
      device_interface.print_cheque_sell(goods, payments, fiscal_props)
      last_check_info = device_interface.last_check_info

      expect(last_check_info[:sum]).to eq(payments.reduce(0){|acc, pay| acc + pay[:sum]})
      expect(last_check_info[:datetime].to_date).to eq(Date.today)
    end

    it 'should return correct sums for different payments' do
      device_interface.print_cheque_sell(goods, payments, fiscal_props)

      expect(device_interface.payment_register(Kkm::Constants::PaymentType::CASH)).to eq(payments.first[:sum])
      expect(device_interface.payment_register(Kkm::Constants::PaymentType::CASHLESS)).to eq(payments.last[:sum])
    end
  end

  context '#print_report' do
    it 'should print correct report' do
      device_interface.print_cheque_sell(goods, payments, fiscal_props)
      device_interface.print_report Kkm::Constants::ReportType::REPORT_Z

      expect(device_interface.get_report_type).to eq(Kkm::Constants::ReportType::REPORT_Z)
    end

    it 'should use correct mode' do
      device_interface.print_cheque_sell(goods, payments, fiscal_props)
      device_interface.print_report Kkm::Constants::ReportType::REPORT_X, Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR

      expect(device_interface.get_mode).to eq(Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR)
      expect(device_interface.get_report_type).to eq(Kkm::Constants::ReportType::REPORT_X)
    end
  end

  context '#close_day' do
    it 'should print z_report with correct cashier' do
      device_interface.print_cheque_sell(goods, payments, fiscal_props)
      device_interface.close_day(fiscal_props)

      expect(device_interface.get_report_type).to eq(Kkm::Constants::ReportType::REPORT_Z)
    end
  end
end
