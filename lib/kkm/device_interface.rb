class Kkm::DeviceInterface < Kkm::DeviceDriver
  include Kkm::Constants

  def initialize settings
    if settings.is_a? Hash
      settings_xml = Builder::XmlMarkup.new(:indent => 2)
      settings_xml.instruct!
      settings_xml.settings{
        settings.each do |key, val|
          settings_xml.value(val, :name => key)
        end
      }
    else
      settings_xml = settings
    end

    super settings_xml
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

  def open_check_sell
    setup_mode Mode::MODE_REGISTRATION
    put_check_type ChequeType::CHEQUE_SELL
    open_check
  end

  # Все возможные значения указаны в Kkm::Constants::Mode
  def setup_mode mode
    put_mode mode
    set_mode
  end

  # Передавать товар в формате :quantity, :name, :price, :tax
  def register_goods goods
    put_tax_number TaxNumber.tax_number_by_tax(goods[:tax])
    put_quantity goods[:quantity]
    put_price goods[:price]
    put_text_wrap TextWrap::TEXT_WRAP_WORD
    put_name goods[:name]
    registration
  end

  # Передавать товар в формате :quantity, :name, :price, :tax
  # По умолчанию платеж Нал
  def print_goods_check goods, summ, type = PaymentType::CASH, fiscal_props = []
    open_check_sell
    fiscal_props.each do |fiscal_prop|
      setup_fiscal_property fiscal_prop
    end
    goods.each do |goods_item|
      register_goods goods_item
    end
    pay_for_goods summ, type
    close_check
  rescue => e
    # Если возникли ошибки, то аннулируем чек
    new_document
    raise Kkm::Errors::PaymentError, e
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
end
