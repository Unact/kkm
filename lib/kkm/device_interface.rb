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

  # Передавать платеж в формате :sum, :type
  # По умолчанию платеж Нал
  def pay_for_goods pay
    put_summ pay[:sum]
    put_type_close pay[:type] || PaymentType::CASH
    payment
  end

  def print_report report_type, mode = Mode::MODE_REPORT_CLEAR
    setup_mode mode
    put_report_type report_type
    report
  end

  def open_cheque_by_type cheque_type, fiscal_props = [], print_cheque = false
    setup_mode Mode::MODE_REGISTRATION
    setup_cashier fiscal_props
    put_check_type cheque_type
    put_print_check print_cheque
    open_check
  end

  # Все возможные значения указаны в Kkm::Constants::Mode
  def setup_mode mode
    put_mode mode
    set_mode
  end

  # Передавать товар в формате
  # :enable_check_summ, :text_wrap, :quantity, :name, :price, :tax, :position_type, :position_payment_type
  def setup_goods_info goods
    put_enable_check_summ goods[:enable_check_summ] || false
    put_tax_number goods[:tax] || TaxNumber::TAX_NONE
    put_quantity goods[:quantity]
    put_price goods[:price]
    put_position_sum goods[:price] * goods[:quantity]
    put_position_type goods[:position_type] || PositionType::GOODS
    put_position_payment_type goods[:position_payment_type] || PositionPaymentType::FULL_PAYMENT
    put_text_wrap goods[:text_wrap] || TextWrap::TEXT_WRAP_WORD
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

  def print_cheque_sell goods, payments = [], fiscal_props = [], print_cheque = false
    open_cheque_by_type ChequeType::CHEQUE_SELL, fiscal_props, print_cheque
    fiscal_props.each{|fiscal_prop| setup_fiscal_property(fiscal_prop)}
    goods.each{|goods_item| register_goods(goods_item)}
    payments.each{|pay| pay_for_goods(pay)}
    close_check
  rescue => e
    # Если возникли ошибки, то аннулируем чек
    new_document
    raise e
  end

  def print_cheque_sell_return goods, payments = [], fiscal_props = [], print_cheque = false
    open_cheque_by_type ChequeType::CHEQUE_SELL_RETURN, fiscal_props, print_cheque
    fiscal_props.each{|fiscal_prop| setup_fiscal_property(fiscal_prop)}
    goods.each{|goods_item| return_goods(goods_item)}
    payments.each{|pay| pay_for_goods(pay)}
    close_check
  rescue => e
    # Если возникли ошибки, то аннулируем чек
    new_document
    raise e
  end

  def print_text text = "", alignment = Alignment::ALIGNMENT_CENTER, wrap = TextWrap::TEXT_WRAP_WORD
    put_caption text
    put_text_wrap wrap
    put_alignment alignment
    print_string
  end

  def print_slip &block
    yield self
    (0..CHEQUE_CUT_LINES).each{ print_text }
    partial_cut
  end

  def close_day fiscal_props = []
    setup_cashier fiscal_props
    print_report(Kkm::Constants::ReportType::REPORT_Z)
  end

  def setup_cashier fiscal_props
    cashier_fiscal_prop = fiscal_props.find{|prop| prop[:number] == Kkm::Constants::FiscalProperty::CASHIER}
    setup_fiscal_property(cashier_fiscal_prop) if cashier_fiscal_prop
  end

  # Установить фискальный параметр в ККМ
  # Передавать параметр в формате :number, :value, :type, :print, :properties
  # По умолчанию :type = FiscalPropertyType::STRING
  # По умолчанию :print = true
  # Если задан :properties, то переходим в режим создания составного тега
  def setup_fiscal_property property, write = true
    number = property[:number]
    value = property[:value]
    type = property[:type] || FiscalPropertyType::STRING

    if property.has_key?(:properties)
      begin_form_fiscal_property
      property[:properties].each{|fiscal_prop| setup_fiscal_property(fiscal_prop, false)}
      end_form_fiscal_property

      value = get_fiscal_property_value
      type = FiscalPropertyType::RAW
    end

    put_fiscal_property_type(type)
    put_fiscal_property_print(property.has_key?(:print) ? property[:print] : true)
    put_fiscal_property_number(number)
    put_fiscal_property_value(value)
    write ? write_fiscal_property : add_fiscal_property
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
    kkm_status
    nil
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

  def payment_register payment_type = PaymentType::CASH, check_type = ChequeType::CHEQUE_SELL
    put_check_type check_type
    put_type_close payment_type
    get_register_by_number RegisterNumber::PAYMENT_SUM
    get_summ
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

  def machine_number
    get_register_by_number RegisterNumber::FISCAL_PARAMS
    get_machine_number.strip
  end

  def factory_number
    get_register_by_number RegisterNumber::FACTORY_NUMBER
    get_serial_number
  end

  def fiscal_storage_number
    get_register_by_number RegisterNumber::FISCAL_STORAGE_NUMBER
    get_serial_number
  end

  def current_datetime
    get_register_by_number RegisterNumber::CURRENT_DATETIME
    get_time
  end

  # Номер ИНН указанный при фискализации
  def fiscal_inn
    get_register_by_number RegisterNumber::FISCAL_PARAMS
    get_inn
  end

  # Сквозной номер документа
  def current_doc_number
    get_register_by_number RegisterNumber::CURRENT_CHECK_DOC
    get_doc_number
  end

  # Номер смены последней смены ФН
  def last_check_session_number
    get_register_by_number RegisterNumber::LAST_CHECK_SESSION
    get_session
  end

  # Номер чека последней смены ФН
  def last_check_session_check_number
    get_register_by_number RegisterNumber::LAST_CHECK_SESSION
    get_check_number
  end

  # Информация о закрытости смены
  def current_close_session
    get_register_by_number RegisterNumber::CURRENT_CLOSE_SESSION
    {
      opened: get_session_opened,
      datetime: get_time
    }
  end

  # Информация о версии ФН
  def fn_version
    res = execute_command(Command::FN_VERSION)

    {
      version: [res[2..17].join].pack('H*').delete("\x00"),
      debug: Integer(res[18], 16) == 0
    }
  end

  # Информация о статусе информационного обмена с ОФД
  def ofd_status
    res = execute_command(Command::OFD_STATUS)

    {
      state: Integer(res[2], 16),
      read_message: res[3].to_i == 1,
      to_send: Integer(res[4..5].reverse.join, 16),
      first_doc: Integer(res[6..9].reverse.join, 16)
    }
  end

  # Информация о статусе ККТ
  def kkm_status
    res = execute_command(Command::KKM_STATUS)

    {
      cashier: res[1],
      operator: Integer(res[2], 16),
      datetime: Time.new("20#{res[3]}".to_i, res[4].to_i, res[5].to_i, res[6].to_i, res[7].to_i, res[8].to_i),
      flags: Integer(res[9], 16),
      model: Integer(res[14], 16),
      os_version: res[16].split('').join('.'),
      mode: res[17][1],
      advanced_mode: res[17][0],
      doc_number: (res[18..19].reverse.join).to_i,
      session_number: (res[20..21].reverse.join).to_i,
      cheque_status: res[22].to_i,
      cheque_summ: res[23..27].join.to_i,
      floating_point: res[28].to_i,
      port_type: res[29].to_i
    }
  end

  # Информация о статусе ФН
  def fn_status
    res = execute_command(Command::FN_STATUS)

    {
      phase: res[2].to_i,
      document: Integer(res[3], 16),
      has_document_data: res[4].to_i == 1,
      session_opened: res[5].to_i == 1,
      flags: Integer(res[6], 16),
      fiscal_storage_number: [res[12..27].join].pack('H*')
    }
  end

  # Информация о текущем режиме
  def current_mode
    res = execute_command(Command::KKM_STATUS_CODE)

    {
      mode: Integer(res[1][0], 16),
      advanced_mode: Integer(res[1][1], 16),
      short_flags: Integer(res[2], 16)
    }
  end

  def get_register_by_number number
    put_register_number number
    get_register
  end

  # Выолнить команду протокола Атол 3.1
  def execute_command command
    put_need_result_flag true
    put_command_buffer command
    run_command
    get_answer_buffer.split
  end
end
