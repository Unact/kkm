# frozen_string_literal: true

module KKM
  # Class is intended for printing receipts, closing shifts and getting information from KKM device
  class Device
    include DeviceMixins::Command
    include DeviceMixins::Data
    include DeviceMixins::FNData
    include DeviceMixins::Receipt
    include DeviceMixins::Report
    include DeviceMixins::Text

    attr_accessor :device_name
    attr_reader :device_id, :timeout

    def initialize(settings)
      raise TypeError, "Parameter must be an Models::Settings" unless settings.is_a?(Models::Settings)

      settings = settings.raw_settings
      @device_id = settings.delete("DeviceId")
      @device_name = settings.delete("DeviceName") || Constants::DEFAULT_DEVICE_NAME
      @timeout = settings.delete("Timeout") || Constants::DEFAULT_CONNECTION_TIMEOUT
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

    def setup_operator(operator)
      return if operator.nil?
      raise TypeError, "Parameter must be an Models::Operator or nil" unless operator.is_a?(Models::Operator)

      tags = []
      tags.push(Models::Tag.new(Constants::Tag::OPERATOR, operator.name)) unless operator.name.nil?
      tags.push(Models::Tag.new(Constants::Tag::OPERATOR_VATIN, operator.vatin)) unless operator.vatin.nil?

      setup_tags(tags)
      operator_login
    end

    def open_day(operator = nil, electronically: true)
      setup_operator(operator)

      set_param(LibFptr::LIBFPTR_PARAM_REPORT_ELECTRONICALLY, electronically)
      open_shift
    rescue DeviceError => e
      raise e if e.code != LibFptr::LIBFPTR_ERROR_DENIED_IN_OPENED_SHIFT
    end

    def close_day(operator = nil, electronically: true)
      print_report(Models::Report.new(LibFptr::LIBFPTR_RT_CLOSE_SHIFT, operator), electronically: electronically)
    rescue DeviceError => e
      raise e if e.code != LibFptr::LIBFPTR_ERROR_DENIED_IN_CLOSED_SHIFT
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

    # rubocop:disable Naming/PredicateName
    def is_opened
      raise_error if @ifptr.is_opened != LibFptr::LIBFPTR_OK
    end
    # rubocop:enable Naming/PredicateName

    private

    def setup_tags(tags)
      raise TypeError, "Parameter must be an Models::Tag array" if tags.any? { |tag| !tag.is_a?(Models::Tag) }

      tags
        .sort { |tag| !tag.tags.empty? ? -1 : 1 }
        .each { |tag| setup_tag(tag) }
    end

    def setup_tag(tag)
      number = tag.number
      value = tag.value

      unless tag.tags.empty?
        util_form_tlv
        setup_tags(tag.tags)
        util_form_tlv

        value = get_param_bytearray(LibFptr::LIBFPTR_PARAM_TAG_VALUE)
      end

      if tag.print
        set_param(number, value)
      else
        set_non_printable_param(number, value)
      end
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
