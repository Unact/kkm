require 'helpers/spec_helper'

RSpec.describe Kkm::DeviceInterface do
  before :all do
    device_settings = YAML.load_file(File.open('./spec/device_settings/kkm.yaml'))
    @test_device_interface = Kkm::DeviceInterface.new device_settings
    @test_device_interface.turn_on
    @test_goods = [
      {:name => 'Test Goods 1', :quantity => 10, :price => 12, :tax => :'10'},
      {:name => 'Test Goods 2', :quantity => 20, :price => 20, :tax => :'18'}
    ]
    @test_fiscal_props = [
      {
        :number => Kkm::Constants::FiscalProperty::CASHIER,
        :value => "Тестовый",
        :type => Kkm::Constants::FiscalPropertyType::STRING
      }
    ]
    @test_payment_summ = 520
  end

  after :all do
    @test_device_interface.turn_off
  end

  before :each do
    @test_device_interface.new_document
  end

  it 'beep should be heard' do
    expect { @test_device_interface.beep }.to_not raise_error(Kkm::Errors::DeviceDriverError)
  end

  it 'should print text' do
    expect {
      @test_device_interface.print_text "Test text"
    }.to_not raise_error(Kkm::Errors::DeviceDriverError)
  end

  it 'should print correct cheque' do
    expect {
      @test_device_interface.print_cheque_sell @test_goods, @test_payment_summ
    }.to_not raise_error(Kkm::Errors::DeviceDriverError)
    expect(
      @test_device_interface.get_change
    ).to eq(0)
    expect(
      @test_device_interface.get_remainder
    ).to eq(0)
    expect(
      @test_device_interface.get_mode
    ).to eq(Kkm::Constants::Mode::MODE_REGISTRATION)
    expect(
      @test_device_interface.get_check_type
    ).to eq(Kkm::Constants::ChequeType::CHEQUE_SELL)
  end

  it 'should print cashier in cheque' do
    expect {
      @test_device_interface.print_cheque_sell(
        @test_goods,
        @test_payment_summ,
        Kkm::Constants::PaymentType::CASH,
        @test_fiscal_props
      )
    }.to_not raise_error(Kkm::Errors::DeviceDriverError)
  end

  it 'should get correct check info' do
    @test_device_interface.print_cheque_sell(
      @test_goods,
      @test_payment_summ,
      Kkm::Constants::PaymentType::CASH,
      @test_fiscal_props
    )
    last_check_info = @test_device_interface.last_check_info

    expect(last_check_info[:sum]).to eq(@test_payment_summ)
    expect(last_check_info[:datetime].to_date).to eq(Date.today)
  end

  it 'should set correct mode' do
    @test_device_interface.setup_mode Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR
    expect(
      @test_device_interface.get_mode
    ).to eq(Kkm::Constants::Mode::MODE_REPORT_NO_CLEAR)
  end

  it 'should print ReportZ' do
    @test_device_interface.report_z

    expect(
      @test_device_interface.get_mode
    ).to eq(Kkm::Constants::Mode::MODE_REPORT_CLEAR)
    expect(
      @test_device_interface.get_report_type
    ).to eq(Kkm::Constants::ReportType::REPORT_Z)
  end

end
