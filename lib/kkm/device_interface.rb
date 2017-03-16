class Kkm::DeviceInterface < Kkm::DeviceDriver
  include Kkm::Constants

  def initialize settings
    settings_xml = Builder::XmlMarkup.new(:indent => 2)
    settings_xml.instruct!
    settings_xml.settings{
      settings.each do |key, val|
        settings_xml.value(val, :name => key)
      end
    }
    super settings_xml
  end

  # Передавать платеж в формате :sum, :type
  def pay_for_goods goods_payment
    put_summ goods_payment[:sum]
    put_type_close goods_payment[:type]
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
  # Передавать платеж в формате :sum, :type
  def print_goods_check goods, goods_payment
    open_check_sell
    goods.each do |goods_item|
      register_goods goods_item
    end
    pay_for_goods goods_payment
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
end
