require 'nokogiri'
class Kkm::DeviceInterface < Kkm::DeviceDriver
  include Kkm::Constants

  attr_accessor :settings_xml, :settings_hash

  def initialize settings
    @settings_hash = {}
    if settings.is_a? Hash
      @settings_hash = settings
      settings_xml = Nokogiri::XML::Builder.new(encoding: 'UTF-8') do |xml|
        xml.settings {
          settings.each{ |key, val| xml.value val, name: key }
        }
      end
      @settings_xml = settings_xml.to_xml
    else
      @settings_xml = settings
      Nokogiri::XML.parse(@settings_xml).xpath("//settings//value").each do |val|
        @settings_hash[val.attributes["name"].value] = val.text
      end
    end

    super @settings_xml
  end

  def pay_for_goods summ, type
    put_summ summ
    put_type_close type
    payment
  end

  def report_z
    setup_mode Mode::MODE_REPORT_CLEAR
    put_report_type ReportType::REPORT_Z
    report
  end

  def open_cheque_by_type cheque_type, print_cheque = false
    setup_mode Mode::MODE_REGISTRATION
    put_check_type cheque_type
    put_print_check print_cheque
    open_check
  end

  def open_sell_cheque print_cheque = false
    open_cheque_by_type ChequeType::CHEQUE_SELL, print_cheque
  end

  def open_sell_return_cheque print_cheque = false
    open_cheque_by_type ChequeType::CHEQUE_SELL_RETURN, print_cheque
  end

  # Все возможные значения указаны в Kkm::Constants::Mode
  def setup_mode mode
    put_mode mode
    set_mode
  end

  # Передавать товар в формате :quantity, :name, :price, :tax
  def setup_goods_info goods
    put_tax_number TaxNumber.tax_number_by_tax(goods[:tax])
    put_quantity goods[:quantity]
    put_price goods[:price]
    put_text_wrap TextWrap::TEXT_WRAP_WORD
    put_name goods[:name]
  end

  def register_goods goods
    setup_goods_info goods
    registration
  end

  def return_goods goods
    setup_goods_info goods
    return_registration
  end

  # Передавать товар в формате :quantity, :name, :price, :tax
  # По умолчанию платеж Нал
  def print_cheque_sell goods, summ, type = PaymentType::CASH, fiscal_props = [], print_cheque = false
    open_sell_cheque print_cheque

    setup_fiscal_properties fiscal_props
    goods.each do |goods_item|
      register_goods goods_item
    end
    pay_for_goods summ, type
    close_check
  rescue => e
    # Если возникли ошибки, то аннулируем чек
    new_document
    raise e
  end

  # Передавать товар в формате :quantity, :name, :price, :tax
  # По умолчанию платеж Нал
  def print_cheque_sell_return goods, summ, type = PaymentType::CASH, fiscal_props = [], print_cheque = false
    open_sell_return_cheque print_cheque
    setup_fiscal_properties fiscal_props
    goods.each do |goods_item|
      return_goods goods_item
    end
    pay_for_goods summ, type
    close_check
  rescue => e
    # Если возникли ошибки, то аннулируем чек
    new_document
    raise e
  end

  def setup_fiscal_properties fiscal_props = []
    fiscal_props.each do |fiscal_prop|
      setup_fiscal_property fiscal_prop
    end
  end

  def print_text text, alignment = Alignment::ALIGNMENT_CENTER, wrap = TextWrap::TEXT_WRAP_WORD
    put_caption text
    put_text_wrap wrap
    put_alignment alignment
    print_string
  end

  # Установить фискальный параметр в ККМ
  # Передавать параметр в формате :number, :value, :type, :print
  # По умолчанию :type = FiscalPropertyType::STRING
  # По умолчанию :print = true
  def setup_fiscal_property property
    put_fiscal_property_type property[:type] || FiscalPropertyType::STRING
    put_fiscal_property_print property.has_key?(:print) ? property[:print] : true
    put_fiscal_property_number property[:number]
    put_fiscal_property_value property[:value]
    write_fiscal_property
  end

  # Считать фискальный параметр из ККМ
  def read_kkm_fiscal_property number, type = FiscalPropertyType::STRING
    put_fiscal_property_number number
    put_fiscal_property_type type
    read_fiscal_property
    get_fiscal_property_value
  end

  def turn_on
    put_device_enabled true
    get_status
  end

  def turn_off
    put_device_enabled false
  end

  def last_check_sum
    get_register_by_number RegisterNumber::LAST_CHECK_INFO
    get_summ
  end

  def last_check_doc_number
    get_register_by_number RegisterNumber::LAST_CHECK_INFO
    get_doc_number
  end

  def last_check_type
    get_register_by_number RegisterNumber::LAST_CHECK_INFO
    get_check_type
  end

  def last_check_fiscal_doc_number
    get_register_by_number RegisterNumber::LAST_CHECK_INFO
    get_value.to_i
  end

  def last_check_datetime
    get_register_by_number RegisterNumber::LAST_CHECK_INFO
    get_time
  end

  def last_check_info
    {
      sum: last_check_sum,
      fiscal_doc_number: last_check_fiscal_doc_number,
      doc_number: last_check_doc_number,
      check_type: last_check_type,
      datetime: last_check_datetime,
      fiscal_storage_number: fiscal_storage_number
    }
  end

  # В соответствии с документацией
  def last_check_web_params
    "t=#{last_check_datetime.strftime("%Y%m%dT%H%M%S")}&s=#{last_check_sum}" +
    "&fn=#{fiscal_storage_number}&i=#{last_check_doc_number}" +
    "&fp=#{last_check_fiscal_doc_number}&n=#{last_check_type}"
  end

  def fiscal_storage_number
    get_register_by_number RegisterNumber::FISCAL_STORAGE_NUMBER
    get_serial_number
  end

  def get_register_by_number number
    put_register_number number
    get_register
  end
end
