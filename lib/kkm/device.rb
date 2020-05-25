# frozen_string_literal: true

module Kkm
  # Class is intended for printing receipts, closing shifts and getting information from KKM device
  class Device
    attr_accessor :device_name

    def initialize(settings)
      @device_id = settings["DeviceId"]
      @device_name = settings["DeviceName"] || Constants::DEFAULT_DEVICE_NAME
      @timeout = settings["Timeout"] || Constants::DEFAULT_CONNECTION_TIMEOUT
      @ifptr = IFptr.new(@device_id)

      set_settings(settings)
    end

    def work
      turn_on
      yield(self)
    rescue StandardError => e
      raise e
    ensure
      turn_off
    end

    def print_receipt(receipt_type, positions, payments, fiscal_props = [], print_physical = false)
      setup_cashier(fiscal_props)
      setup_fiscal_properties(fiscal_props)

      set_param(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE, receipt_type)
      set_param(LibFptr::LIBFPTR_PARAM_RECEIPT_ELECTRONICALLY, !print_physical)
      open_receipt

      positions.each { |position| register_position(position) }
      payments.each { |payment| register_payment(payment) }

      close_receipt
    rescue DeviceError => e
      cancel_receipt if receipt_data[:receipt_type] != LibFptr::LIBFPTR_RT_CLOSED
      raise e
    end

    def register_position(position_data)
      fiscal_props = position_data[:fiscal_props] || []
      fiscal_props.push(
        {
          number: Constants::FiscalProperty::POSITION_TYPE,
          value: position_data[:position_type] || Constants::PositionType::GOODS
        },
        {
          number: Constants::FiscalProperty::POSITION_PAYMENT_TYPE,
          value: position_data[:position_payment_type] || Constants::PositionPaymentType::FULL_PAYMENT
        }
      )

      setup_fiscal_properties(fiscal_props)
      set_param(LibFptr::LIBFPTR_PARAM_COMMODITY_NAME, position_data[:name])
      set_param(LibFptr::LIBFPTR_PARAM_PRICE, position_data[:price])
      set_param(LibFptr::LIBFPTR_PARAM_QUANTITY, position_data[:quantity])
      set_param(LibFptr::LIBFPTR_PARAM_TAX_TYPE, position_data[:tax] || LibFptr::LIBFPTR_TAX_NO)

      registration
    end

    def register_payment(payment_data)
      set_param(LibFptr::LIBFPTR_PARAM_PAYMENT_SUM, payment_data[:sum])
      set_param(LibFptr::LIBFPTR_PARAM_PAYMENT_TYPE, payment_data[:type] || LibFptr::LIBFPTR_PT_CASH)
      payment
    end

    def status_data
      set_param(LibFptr::LIBFPTR_PARAM_DATA_TYPE, LibFptr::LIBFPTR_DT_STATUS)
      query_data

      {
        operator_id: get_param_int(LibFptr::LIBFPTR_PARAM_OPERATOR_ID),
        logical_number: get_param_int(LibFptr::LIBFPTR_PARAM_LOGICAL_NUMBER),
        shift_state: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_STATE),
        model: get_param_int(LibFptr::LIBFPTR_PARAM_MODEL),
        mode: get_param_int(LibFptr::LIBFPTR_PARAM_MODE),
        submode: get_param_int(LibFptr::LIBFPTR_PARAM_SUBMODE),
        receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER),
        document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
        shift_number: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_NUMBER),
        receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
        receipt_line_length: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH),
        receipt_line_length_pix: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_LINE_LENGTH_PIX),
        receipt_sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
        datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
        fiscal: get_param_bool(LibFptr::LIBFPTR_PARAM_FISCAL),
        fn_fiscal: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_FISCAL),
        fn_present: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_PRESENT),
        invalid_fn: get_param_bool(LibFptr::LIBFPTR_PARAM_INVALID_FN),
        cashdrawer_opened: get_param_bool(LibFptr::LIBFPTR_PARAM_CASHDRAWER_OPENED),
        paper_present: get_param_bool(LibFptr::LIBFPTR_PARAM_RECEIPT_PAPER_PRESENT),
        paper_near_end: get_param_bool(LibFptr::LIBFPTR_PARAM_PAPER_NEAR_END),
        cover_opened: get_param_bool(LibFptr::LIBFPTR_PARAM_COVER_OPENED),
        printer_connection_lost: get_param_bool(LibFptr::LIBFPTR_PARAM_PRINTER_CONNECTION_LOST),
        printer_error: get_param_bool(LibFptr::LIBFPTR_PARAM_PRINTER_ERROR),
        cut_error: get_param_bool(LibFptr::LIBFPTR_PARAM_CUT_ERROR),
        printer_overheat: get_param_bool(LibFptr::LIBFPTR_PARAM_PRINTER_OVERHEAT),
        blocked: get_param_bool(LibFptr::LIBFPTR_PARAM_BLOCKED),
        serial_number: get_param_str(LibFptr::LIBFPTR_PARAM_SERIAL_NUMBER),
        model_name: get_param_str(LibFptr::LIBFPTR_PARAM_MODEL_NAME),
        unit_version: get_param_str(LibFptr::LIBFPTR_PARAM_UNIT_VERSION)
      }
    end

    def datetime
      set_param(LibFptr::LIBFPTR_PARAM_DATA_TYPE, LibFptr::LIBFPTR_DT_DATE_TIME)
      query_data

      get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME)
    end

    def payment_register_data(payment_type = LibFptr::LIBFPTR_PT_CASH, receipt_type = LibFptr::LIBFPTR_RT_SELL)
      set_param(LibFptr::LIBFPTR_PARAM_DATA_TYPE, LibFptr::LIBFPTR_DT_PAYMENT_SUM)
      set_param(LibFptr::LIBFPTR_PARAM_PAYMENT_TYPE, payment_type)
      set_param(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE, receipt_type)
      query_data

      get_param_double(LibFptr::LIBFPTR_PARAM_SUM)
    end

    def shift_state_data
      set_param(LibFptr::LIBFPTR_PARAM_DATA_TYPE, LibFptr::LIBFPTR_DT_SHIFT_STATE)
      query_data

      {
        opened: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_STATE) == LibFptr::LIBFPTR_SS_OPENED,
        datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME)
      }
    end

    def receipt_data
      set_param(LibFptr::LIBFPTR_PARAM_DATA_TYPE, LibFptr::LIBFPTR_DT_RECEIPT_STATE)
      query_data

      {
        receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
        receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER),
        document_number: get_param_str(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER),
        receipt_sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
        remainder: get_param_double(LibFptr::LIBFPTR_PARAM_REMAINDER),
        change: get_param_double(LibFptr::LIBFPTR_PARAM_CHANGE)
      }
    end

    def fn_last_receipt_data
      set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, LibFptr::LIBFPTR_FNDT_LAST_RECEIPT)
      fn_query_data

      {
        datetime: get_param_datetime(LibFptr::LIBFPTR_PARAM_DATE_TIME),
        sum: get_param_double(LibFptr::LIBFPTR_PARAM_RECEIPT_SUM),
        receipt_type: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE),
        fiscal_sign: get_param_str(LibFptr::LIBFPTR_PARAM_FISCAL_SIGN),
        document_number: get_param_int(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER)
      }
    end

    def fn_info_data
      set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, LibFptr::LIBFPTR_FNDT_FN_INFO)
      fn_query_data

      {
        serial_number: get_param_str(LibFptr::LIBFPTR_PARAM_SERIAL_NUMBER),
        version: get_param_str(LibFptr::LIBFPTR_PARAM_FN_VERSION),
        type: get_param_int(LibFptr::LIBFPTR_PARAM_FN_TYPE),
        state: get_param_int(LibFptr::LIBFPTR_PARAM_FN_STATE),
        flags: get_param_int(LibFptr::LIBFPTR_PARAM_FN_FLAGS),
        need_replacement: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_NEED_REPLACEMENT),
        resource_exhausted: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_RESOURCE_EXHAUSTED),
        memory_overflow: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_MEMORY_OVERFLOW),
        ofd_timeout: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_OFD_TIMEOUT),
        critical_error: get_param_bool(LibFptr::LIBFPTR_PARAM_FN_CRITICAL_ERROR)
      }
    end

    def fn_registration_data
      set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, LibFptr::LIBFPTR_FNDT_REG_INFO)
      fn_query_data

      {
        taxation_types: get_param_int(Constants::FiscalProperty::TAXATION_TYPES),
        agent_type: get_param_int(Constants::FiscalProperty::AGENT_TYPE),
        ffd_version: get_param_int(Constants::FiscalProperty::FFD_VERSION),
        auto_mode: get_param_bool(Constants::FiscalProperty::AUTO_MODE),
        offline_mode: get_param_bool(Constants::FiscalProperty::OFFLINE_MODE),
        encryption: get_param_bool(Constants::FiscalProperty::ENCRYPTION),
        internet: get_param_bool(Constants::FiscalProperty::INTERNET),
        service: get_param_bool(Constants::FiscalProperty::SERVICE),
        bso: get_param_bool(Constants::FiscalProperty::BSO),
        lottery: get_param_bool(Constants::FiscalProperty::LOTTERY),
        gambling: get_param_bool(Constants::FiscalProperty::GAMBLING),
        excise: get_param_bool(Constants::FiscalProperty::EXCISE),
        machine_installation: get_param_bool(Constants::FiscalProperty::MACHINE_INSTALLATION),
        organization_inn: get_param_str(Constants::FiscalProperty::ORGANIZATION_INN),
        organization_name: get_param_str(Constants::FiscalProperty::ORGANIZATION_NAME),
        organization_email: get_param_str(Constants::FiscalProperty::ORGANIZATION_EMAIL),
        payments_address: get_param_str(Constants::FiscalProperty::PAYMENTS_ADDRESS),
        payments_place: get_param_str(Constants::FiscalProperty::PAYMENTS_PLACE),
        registration_number: get_param_str(Constants::FiscalProperty::REGISTRATION_NUMBER),
        machine_number: get_param_str(Constants::FiscalProperty::MACHINE_NUMBER),
        ofd_inn: get_param_str(Constants::FiscalProperty::OFD_INN),
        ofd_name: get_param_str(Constants::FiscalProperty::OFD_NAME)
      }
    end

    def fn_shift_data
      set_param(LibFptr::LIBFPTR_PARAM_FN_DATA_TYPE, LibFptr::LIBFPTR_FNDT_SHIFT)
      fn_query_data

      {
        shift_number: get_param_int(LibFptr::LIBFPTR_PARAM_SHIFT_NUMBER),
        receipt_number: get_param_int(LibFptr::LIBFPTR_PARAM_RECEIPT_NUMBER)
      }
    end

    def print_report(report_type, document_number = nil)
      set_param(LibFptr::LIBFPTR_PARAM_REPORT_TYPE, report_type)
      set_param(LibFptr::LIBFPTR_PARAM_DOCUMENT_NUMBER, document_number) unless document_number.nil?
      report
    end

    def open_day(fiscal_props = [], print_physical = false)
      setup_cashier(fiscal_props)

      set_param(LibFptr::LIBFPTR_PARAM_REPORT_ELECTRONICALLY, !print_physical)
      open_shift
    end

    def close_day(fiscal_props = [], print_physical = false)
      setup_cashier(fiscal_props)

      set_param(LibFptr::LIBFPTR_PARAM_REPORT_ELECTRONICALLY, !print_physical)
      print_report(LibFptr::LIBFPTR_RT_CLOSE_SHIFT)
    end

    def print_text_line(text = "", alignment = LibFptr::LIBFPTR_ALIGNMENT_CENTER, wrap = LibFptr::LIBFPTR_TW_WORDS)
      set_param(LibFptr::LIBFPTR_PARAM_TEXT, text)
      set_param(LibFptr::LIBFPTR_PARAM_ALIGNMENT, alignment)
      set_param(LibFptr::LIBFPTR_PARAM_TEXT_WRAP, wrap)

      print_text
    end

    def print_slip
      yield(self)
      (0..Constants::CHEQUE_CUT_LINES).each { print_text_line }
      set_param(LibFptr::LIBFPTR_PARAM_CUT_TYPE, LibFptr::LIBFPTR_CT_PART)
      cut
    end

    def status_raw_data
      res = execute_command(Constants::Command::STATUS)

      {
        cashier: res[1],
        operator: Integer(res[2], 16),
        datetime: Time.new("20#{res[3]}".to_i, res[4].to_i, res[5].to_i, res[6].to_i, res[7].to_i, res[8].to_i),
        flags: Integer(res[9], 16),
        model: Integer(res[14], 16),
        os_version: res[16].split("").join("."),
        mode: res[17][1],
        advanced_mode: res[17][0],
        doc_number: res[18..19].reverse.join.to_i,
        session_number: res[20..21].reverse.join.to_i,
        cheque_status: res[22].to_i,
        cheque_summ: res[23..27].join.to_i,
        floating_point: res[28].to_i,
        port_type: res[29].to_i
      }
    end

    def fn_status_raw_data
      res = execute_command(Constants::Command::FN_STATUS)

      {
        phase: res[2].to_i,
        document: Integer(res[3], 16),
        has_document_data: res[4].to_i == 1,
        session_opened: res[5].to_i == 1,
        flags: Integer(res[6], 16),
        fiscal_storage_number: [res[12..27].join].pack("H*")
      }
    end

    def ofd_status_raw_data
      res = execute_command(Constants::Command::OFD_STATUS)

      {
        state: Integer(res[2], 16),
        read_message: res[3].to_i == 1,
        to_send: Integer(res[4..5].reverse.join, 16),
        first_doc: Integer(res[6..9].reverse.join, 16)
      }
    end

    def mode_raw_data
      res = execute_command(Constants::Command::STATUS_CODE)

      {
        mode: Integer(res[1][1], 16),
        advanced_mode: Integer(res[1][0], 16),
        short_flags: Integer(res[2], 16)
      }
    end

    def version
      @ifptr.version
    end

    def turn_on
      process_time = Process.clock_gettime(Process::CLOCK_MONOTONIC)

      begin
        open
      rescue DeviceError => e
        raise e if e.code != LibFptr::LIBFPTR_ERROR_NO_CONNECTION
        raise e if Process.clock_gettime(Process::CLOCK_MONOTONIC) >= process_time + @timeout / 1000.0

        retry
      end
    end

    def turn_off
      close
    end

    # rubocop:disable PredicateName
    def is_opened
      raise_error if @ifptr.is_opened != LibFptr::LIBFPTR_OK
    end
    # rubocop:enable PredicateName

    private

    def setup_cashier(fiscal_props)
      cashier_fiscal_props = fiscal_props.select do |prop|
        prop[:number] == Kkm::Constants::FiscalProperty::CASHIER ||
          prop[:number] == Kkm::Constants::FiscalProperty::CASHIER_INN
      end

      return if cashier_fiscal_props.empty?

      setup_fiscal_properties(cashier_fiscal_props)
      operator_login
    end

    def setup_fiscal_properties(fiscal_properties)
      fiscal_properties
        .sort { |fiscal_prop| fiscal_prop.key?(:fiscal_props) ? -1 : 1 }
        .each { |fiscal_prop| setup_fiscal_property(fiscal_prop) }
    end

    def setup_fiscal_property(property, write = true)
      number = property[:number]
      value = property[:value]

      if property.key?(:fiscal_props)
        util_form_tlv
        setup_fiscal_properties(property[:fiscal_props])
        util_form_tlv

        value = get_param_bytearray(LibFptr::LIBFPTR_PARAM_TAG_VALUE)
      end

      if write
        set_param(number, value)
      else
        set_non_printable_param(number, value)
      end
    end

    def execute_command(command, wait = true)
      command_bytearray = command.split.map { |part| Integer(part, 16) }
      set_param(LibFptr::LIBFPTR_PARAM_COMMAND_BUFFER, command_bytearray)
      set_param(LibFptr::LIBFPTR_PARAM_TIMEOUT_ENQ, Constants::DEFAULT_TIMEOUT_ENQ)
      set_param(LibFptr::LIBFPTR_PARAM_NO_NEED_ANSWER, !wait)
      run_command

      get_param_bytearray(LibFptr::LIBFPTR_PARAM_ANSWER_BUFFER).map { |v| v.to_s(16).rjust(2, "0").upcase }
    end

    def set_settings(settings)
      raise_error if @ifptr.set_settings(settings) != LibFptr::LIBFPTR_OK
    end

    def get_settings
      @ifptr.get_settings
    end

    def get_single_setting(key)
      @ifptr.get_single_setting(key)
    end

    def set_single_setting(key, value)
      @ifptr.set_single_setting(key, value)
    end

    def apply_single_settings
      raise_error if @ifptr.apply_single_settings != LibFptr::LIBFPTR_OK
    end

    def open
      raise_error if @ifptr.open != LibFptr::LIBFPTR_OK
    end

    def close
      raise_error if @ifptr.close != LibFptr::LIBFPTR_OK
    end

    def error_code
      @ifptr.error_code
    end

    def error_description
      @ifptr.error_description
    end

    def reset_error
      @ifptr.reset_error
    end

    def set_param_bool(param_id, value)
      @ifptr.set_param_bool(param_id, value)
    end

    def set_param_int(param_id, value)
      @ifptr.set_param_int(param_id, value)
    end

    def set_param_double(param_id, value)
      @ifptr.set_param_double(param_id, value)
    end

    def set_param_str(param_id, value)
      @ifptr.set_param_str(param_id, value)
    end

    def set_param_datetime(param_id, value)
      @ifptr.set_param_datetime(param_id, value)
    end

    def set_param_bytearray(param_id, value)
      @ifptr.set_param_bytearray(param_id, value)
    end

    def set_param(param_id, value)
      @ifptr.set_param(param_id, value)
    end

    def set_user_param_bool(param_id, value)
      @ifptr.set_user_param_bool(param_id, value)
    end

    def set_user_param_int(param_id, value)
      @ifptr.set_user_param_int(param_id, value)
    end

    def set_user_param_double(param_id, value)
      @ifptr.set_user_param_double(param_id, value)
    end

    def set_user_param_str(param_id, value)
      @ifptr.set_user_param_str(param_id, value)
    end

    def set_user_param_datetime(param_id, value)
      @ifptr.set_user_param_datetime(param_id, value)
    end

    def set_user_param_bytearray(param_id, value)
      @ifptr.set_user_param_bytearray(param_id, value)
    end

    def set_user_param(param_id, value)
      @ifptr.set_user_param(param_id, value)
    end

    def set_non_printable_param_bool(param_id, value)
      @ifptr.set_non_printable_param_bool(param_id, value)
    end

    def set_non_printable_param_int(param_id, value)
      @ifptr.set_non_printable_param_int(param_id, value)
    end

    def set_non_printable_param_double(param_id, value)
      @ifptr.set_non_printable_param_double(param_id, value)
    end

    def set_non_printable_param_str(param_id, value)
      @ifptr.set_non_printable_param_str(param_id, value)
    end

    def set_non_printable_param_datetime(param_id, value)
      @ifptr.set_non_printable_param_datetime(param_id, value)
    end

    def set_non_printable_param_bytearray(param_id, value)
      @ifptr.set_non_printable_param_bytearray(param_id, value)
    end

    def set_non_printable_param(param_id, value)
      @ifptr.set_non_printable_param(param_id, value)
    end

    def get_param_bool(param_id)
      @ifptr.get_param_bool(param_id)
    end

    def get_param_int(param_id)
      @ifptr.get_param_int(param_id)
    end

    def get_param_double(param_id)
      @ifptr.get_param_double(param_id)
    end

    def get_param_str(param_id)
      @ifptr.get_param_str(param_id)
    end

    def get_param_datetime(param_id)
      @ifptr.get_param_datetime(param_id)
    end

    def get_param_bytearray(param_id)
      @ifptr.get_param_bytearray(param_id)
    end

    def reset_params
      raise_error if @ifptr.reset_params != LibFptr::LIBFPTR_OK
    end

    def run_command
      raise_error if @ifptr.run_command != LibFptr::LIBFPTR_OK
    end

    def beep
      raise_error if @ifptr.beep != LibFptr::LIBFPTR_OK
    end

    def open_drawer
      raise_error if @ifptr.open_drawer != LibFptr::LIBFPTR_OK
    end

    def cut
      raise_error if @ifptr.cut != LibFptr::LIBFPTR_OK
    end

    def device_poweroff
      raise_error if @ifptr.device_poweroff != LibFptr::LIBFPTR_OK
    end

    def device_reboot
      raise_error if @ifptr.device_reboot != LibFptr::LIBFPTR_OK
    end

    def open_shift
      raise_error if @ifptr.open_shift != LibFptr::LIBFPTR_OK
    end

    def reset_summary
      raise_error if @ifptr.reset_summary != LibFptr::LIBFPTR_OK
    end

    def init_device
      raise_error if @ifptr.init_device != LibFptr::LIBFPTR_OK
    end

    def query_data
      raise_error if @ifptr.query_data != LibFptr::LIBFPTR_OK
    end

    def cash_income
      raise_error if @ifptr.cash_income != LibFptr::LIBFPTR_OK
    end

    def cash_outcome
      raise_error if @ifptr.cash_outcome != LibFptr::LIBFPTR_OK
    end

    def open_receipt
      raise_error if @ifptr.open_receipt != LibFptr::LIBFPTR_OK
    end

    def cancel_receipt
      raise_error if @ifptr.cancel_receipt != LibFptr::LIBFPTR_OK
    end

    def close_receipt
      raise_error if @ifptr.close_receipt != LibFptr::LIBFPTR_OK
    end

    def check_document_closed
      raise_error if @ifptr.check_document_closed != LibFptr::LIBFPTR_OK
    end

    def receipt_total
      raise_error if @ifptr.receipt_total != LibFptr::LIBFPTR_OK
    end

    def receipt_tax
      raise_error if @ifptr.receipt_tax != LibFptr::LIBFPTR_OK
    end

    def registration
      raise_error if @ifptr.registration != LibFptr::LIBFPTR_OK
    end

    def payment
      raise_error if @ifptr.payment != LibFptr::LIBFPTR_OK
    end

    def report
      raise_error if @ifptr.report != LibFptr::LIBFPTR_OK
    end

    def print_text
      raise_error if @ifptr.print_text != LibFptr::LIBFPTR_OK
    end

    def print_cliche
      raise_error if @ifptr.print_cliche != LibFptr::LIBFPTR_OK
    end

    def begin_nonfiscal_document
      raise_error if @ifptr.begin_nonfiscal_document != LibFptr::LIBFPTR_OK
    end

    def end_nonfiscal_document
      raise_error if @ifptr.end_nonfiscal_document != LibFptr::LIBFPTR_OK
    end

    def print_barcode
      raise_error if @ifptr.print_barcode != LibFptr::LIBFPTR_OK
    end

    def print_picture
      raise_error if @ifptr.print_picture != LibFptr::LIBFPTR_OK
    end

    def print_picture_by_number
      raise_error if @ifptr.print_picture_by_number != LibFptr::LIBFPTR_OK
    end

    def upload_picture_from_file
      raise_error if @ifptr.upload_picture_from_file != LibFptr::LIBFPTR_OK
    end

    def download_picture
      raise_error if @ifptr.download_picture != LibFptr::LIBFPTR_OK
    end

    def clear_pictures
      raise_error if @ifptr.clear_pictures != LibFptr::LIBFPTR_OK
    end

    def write_device_setting_raw
      raise_error if @ifptr.write_device_setting_raw != LibFptr::LIBFPTR_OK
    end

    def read_device_setting_raw
      raise_error if @ifptr.read_device_setting_raw != LibFptr::LIBFPTR_OK
    end

    def commit_settings
      raise_error if @ifptr.commit_settings != LibFptr::LIBFPTR_OK
    end

    def init_settings
      raise_error if @ifptr.init_settings != LibFptr::LIBFPTR_OK
    end

    def reset_settings
      raise_error if @ifptr.reset_settings != LibFptr::LIBFPTR_OK
    end

    def write_date_time
      raise_error if @ifptr.write_date_time != LibFptr::LIBFPTR_OK
    end

    def write_license
      raise_error if @ifptr.write_license != LibFptr::LIBFPTR_OK
    end

    def fn_operation
      raise_error if @ifptr.fn_operation != LibFptr::LIBFPTR_OK
    end

    def fn_query_data
      raise_error if @ifptr.fn_query_data != LibFptr::LIBFPTR_OK
    end

    def fn_write_attributes
      raise_error if @ifptr.fn_write_attributes != LibFptr::LIBFPTR_OK
    end

    def external_device_power_on
      raise_error if @ifptr.external_device_power_on != LibFptr::LIBFPTR_OK
    end

    def external_device_power_off
      raise_error if @ifptr.external_device_power_off != LibFptr::LIBFPTR_OK
    end

    def external_device_write_data
      raise_error if @ifptr.external_device_write_data != LibFptr::LIBFPTR_OK
    end

    def external_device_read_data
      raise_error if @ifptr.external_device_read_data != LibFptr::LIBFPTR_OK
    end

    def operator_login
      raise_error if @ifptr.operator_login != LibFptr::LIBFPTR_OK
    end

    def process_json
      raise_error if @ifptr.process_json != LibFptr::LIBFPTR_OK
    end

    def read_device_setting
      raise_error if @ifptr.read_device_setting != LibFptr::LIBFPTR_OK
    end

    def write_device_setting
      raise_error if @ifptr.write_device_setting != LibFptr::LIBFPTR_OK
    end

    def begin_read_records
      raise_error if @ifptr.begin_read_records != LibFptr::LIBFPTR_OK
    end

    def read_next_record
      raise_error if @ifptr.read_next_record != LibFptr::LIBFPTR_OK
    end

    def end_read_records
      raise_error if @ifptr.end_read_records != LibFptr::LIBFPTR_OK
    end

    def user_memory_operation
      raise_error if @ifptr.user_memory_operation != LibFptr::LIBFPTR_OK
    end

    def continue_print
      raise_error if @ifptr.continue_print != LibFptr::LIBFPTR_OK
    end

    def init_mgm
      raise_error if @ifptr.init_mgm != LibFptr::LIBFPTR_OK
    end

    def util_form_tlv
      raise_error if @ifptr.util_form_tlv != LibFptr::LIBFPTR_OK
    end

    def util_mapping
      raise_error if @ifptr.util_mapping != LibFptr::LIBFPTR_OK
    end

    def util_form_nomenclature
      raise_error if @ifptr.util_form_nomenclature != LibFptr::LIBFPTR_OK
    end

    def log_write(tag, level, message)
      raise_error if @ifptr.log_write(tag, level, message) != LibFptr::LIBFPTR_OK
    end

    def show_properties(parent_type, parent)
      raise_error if @ifptr.show_properties(parent_type, parent) != LibFptr::LIBFPTR_OK
    end

    def read_model_flags
      raise_error if @ifptr.read_model_flags != LibFptr::LIBFPTR_OK
    end

    def line_feed
      raise_error if @ifptr.line_feed != LibFptr::LIBFPTR_OK
    end

    def flash_firmware
      raise_error if @ifptr.flash_firmware != LibFptr::LIBFPTR_OK
    end

    def soft_lock_init
      raise_error if @ifptr.soft_lock_init != LibFptr::LIBFPTR_OK
    end

    def soft_lock_query_session_code
      raise_error if @ifptr.soft_lock_query_session_code != LibFptr::LIBFPTR_OK
    end

    def soft_lock_validate
      raise_error if @ifptr.soft_lock_validate != LibFptr::LIBFPTR_OK
    end

    def util_calc_tax
      raise_error if @ifptr.util_calc_tax != LibFptr::LIBFPTR_OK
    end

    def util_tag_info
      raise_error if @ifptr.util_tag_info != LibFptr::LIBFPTR_OK
    end

    def bluetooth_remove_paired_devices
      raise_error if @ifptr.bluetooth_remove_paired_devices != LibFptr::LIBFPTR_OK
    end

    def util_container_versions
      raise_error if @ifptr.util_container_versions != LibFptr::LIBFPTR_OK
    end

    def activate_licenses
      raise_error if @ifptr.activate_licenses != LibFptr::LIBFPTR_OK
    end

    def remove_licenses
      raise_error if @ifptr.remove_licenses != LibFptr::LIBFPTR_OK
    end

    def enter_keys
      raise_error if @ifptr.enter_keys != LibFptr::LIBFPTR_OK
    end

    def validate_keys
      raise_error if @ifptr.validate_keys != LibFptr::LIBFPTR_OK
    end

    def enter_serial_number
      raise_error if @ifptr.enter_serial_number != LibFptr::LIBFPTR_OK
    end

    def get_serial_number_request
      raise_error if @ifptr.get_serial_number_request != LibFptr::LIBFPTR_OK
    end

    def upload_pixel_buffer
      raise_error if @ifptr.upload_pixel_buffer != LibFptr::LIBFPTR_OK
    end

    def download_pixel_buffer
      raise_error if @ifptr.download_pixel_buffer != LibFptr::LIBFPTR_OK
    end

    def print_pixel_buffer
      raise_error if @ifptr.print_pixel_buffer != LibFptr::LIBFPTR_OK
    end

    def util_convert_tag_value
      raise_error if @ifptr.util_convert_tag_value != LibFptr::LIBFPTR_OK
    end

    def parse_marking_code
      raise_error if @ifptr.parse_marking_code != LibFptr::LIBFPTR_OK
    end

    def call_script
      raise_error if @ifptr.call_script != LibFptr::LIBFPTR_OK
    end

    def set_header_lines
      raise_error if @ifptr.set_header_lines != LibFptr::LIBFPTR_OK
    end

    def set_footer_lines
      raise_error if @ifptr.set_footer_lines != LibFptr::LIBFPTR_OK
    end

    def upload_picture_cliche
      raise_error if @ifptr.upload_picture_cliche != LibFptr::LIBFPTR_OK
    end

    def upload_picture_memory
      raise_error if @ifptr.upload_picture_memory != LibFptr::LIBFPTR_OK
    end

    def upload_pixel_buffer_cliche
      raise_error if @ifptr.upload_pixel_buffer_cliche != LibFptr::LIBFPTR_OK
    end

    def upload_pixel_buffer_memory
      raise_error if @ifptr.upload_pixel_buffer_memory != LibFptr::LIBFPTR_OK
    end

    def exec_driver_script
      raise_error if @ifptr.exec_driver_script != LibFptr::LIBFPTR_OK
    end

    def upload_driver_script
      raise_error if @ifptr.upload_driver_script != LibFptr::LIBFPTR_OK
    end

    def exec_driver_script_by_id
      raise_error if @ifptr.exec_driver_script_by_id != LibFptr::LIBFPTR_OK
    end

    def write_universal_counters_settings
      raise_error if @ifptr.write_universal_counters_settings != LibFptr::LIBFPTR_OK
    end

    def read_universal_counters_settings
      raise_error if @ifptr.read_universal_counters_settings != LibFptr::LIBFPTR_OK
    end

    def query_universal_counters_state
      raise_error if @ifptr.query_universal_counters_state != LibFptr::LIBFPTR_OK
    end

    def reset_universal_counters
      raise_error if @ifptr.reset_universal_counters != LibFptr::LIBFPTR_OK
    end

    def cache_universal_counters
      raise_error if @ifptr.cache_universal_counters != LibFptr::LIBFPTR_OK
    end

    def read_universal_counter_sum
      raise_error if @ifptr.read_universal_counter_sum != LibFptr::LIBFPTR_OK
    end

    def read_universal_counter_quantity
      raise_error if @ifptr.read_universal_counter_quantity != LibFptr::LIBFPTR_OK
    end

    def clear_universal_counters_cache
      raise_error if @ifptr.clear_universal_counters_cache != LibFptr::LIBFPTR_OK
    end

    def disable_ofd_channel
      raise_error if @ifptr.disable_ofd_channel != LibFptr::LIBFPTR_OK
    end

    def enable_ofd_channel
      raise_error if @ifptr.enable_ofd_channel != LibFptr::LIBFPTR_OK
    end

    def validate_json
      raise_error if @ifptr.validate_json != LibFptr::LIBFPTR_OK
    end

    def log_write_ex(tag, level, message)
      raise_error if @ifptr.log_write_ex(tag, level, message) != LibFptr::LIBFPTR_OK
    end

    def raise_error
      raise DeviceError.new(error_code, error_description, device_name)
    end
  end
end
