# frozen_string_literal: true

require "json"
require "time"
require "bigdecimal"

module Kkm
  # Ruby implementation of LibFptr
  class IFptr
    def initialize
      interface_pointer = FFI::MemoryPointer.new(:pointer)

      LibFptr.create(interface_pointer)
      @interface = FFI::AutoPointer.new(interface_pointer.get_pointer(0), self.class.method(:finalize))
    end

    def version
      LibFptr.get_version_string(@interface)
    end

    def set_settings(settings)
      LibFptr.set_settings(@interface, LibC.string_to_wchar_pointer(JSON.generate(settings)))
    end

    def get_settings
      ptr = FFI::MemoryPointer.new(:pointer, LibFptr::DEFAULT_BUFF_SIZE)
      size = LibFptr.get_settings(@interface, ptr, ptr.size)

      if size > ptr.size
        ptr = FFI::MemoryPointer.new(:pointer, size)
        LibFptr.get_settings(@interface, ptr, size)
      end

      JSON.parse(LibC.wchar_pointer_to_string(ptr))
    end

    def get_single_setting(key)
      ptr_key = LibC.string_to_wchar_pointer(key)
      ptr = FFI::MemoryPointer.new(:pointer, LibFptr::DEFAULT_BUFF_SIZE)
      size = LibFptr.get_single_setting(@interface, ptr_key, ptr, ptr.size)

      if size > ptr.size
        ptr = FFI::MemoryPointer.new(:pointer, size)
        LibFptr.get_single_setting(@interface, ptr_key, ptr, size)
      end

      LibC.wchar_pointer_to_string(ptr)
    end

    def set_single_setting(key, value)
      LibFptr.set_single_setting(@interface, LibC.string_to_wchar_pointer(key), LibC.string_to_wchar_pointer(value))
    end

    def apply_single_settings
      LibFptr.apply_single_settings(@interface)
    end

    def open
      LibFptr.open(@interface)
    end

    def close
      LibFptr.close(@interface)
    end

    # rubocop:disable PredicateName
    def is_opened
      LibFptr.is_opened(@interface)
    end
    # rubocop:enable PredicateName

    def error_code
      LibFptr.error_code(@interface)
    end

    def error_description
      ptr = FFI::MemoryPointer.new(:pointer, LibFptr::DEFAULT_BUFF_SIZE)
      size = LibFptr.error_description(@interface, ptr, ptr.size)

      if size > ptr.size
        ptr = FFI::MemoryPointer.new(:pointer, size)
        LibFptr.error_description(@interface, ptr, size)
      end

      LibC.wchar_pointer_to_string(ptr)
    end

    def reset_error
      LibFptr.reset_error(@interface)
    end

    def set_param_bool(param_id, value)
      set_method_bool(:set_param, param_id, value)
    end

    def set_param_int(param_id, value)
      set_method_int(:set_param, param_id, value)
    end

    def set_param_double(param_id, value)
      set_method_double(:set_param, param_id, value)
    end

    def set_param_str(param_id, value)
      set_method_str(:set_param, param_id, value)
    end

    def set_param_datetime(param_id, value)
      set_method_datetime(:set_param, param_id, value)
    end

    def set_param_bytearray(param_id, value)
      set_method_bytearray(:set_param, param_id, value)
    end

    def set_param(param_id, value)
      set_method(:set_param, param_id, value)
    end

    def set_user_param_bool(param_id, value)
      set_method_bool(:set_user_param, param_id, value)
    end

    def set_user_param_int(param_id, value)
      set_method_int(:set_user_param, param_id, value)
    end

    def set_user_param_double(param_id, value)
      set_method_double(:set_user_param, param_id, value)
    end

    def set_user_param_str(param_id, value)
      set_method_str(:set_user_param, param_id, value)
    end

    def set_user_param_datetime(param_id, value)
      set_method_datetime(:set_user_param, param_id, value)
    end

    def set_user_param_bytearray(param_id, value)
      set_method_bytearray(:set_user_param, param_id, value)
    end

    def set_user_param(param_id, value)
      set_method(:set_user_param, param_id, value)
    end

    def set_non_printable_param_bool(param_id, value)
      set_method_bool(:set_non_printable_param, param_id, value)
    end

    def set_non_printable_param_int(param_id, value)
      set_method_int(:set_non_printable_param, param_id, value)
    end

    def set_non_printable_param_double(param_id, value)
      set_method_double(:set_non_printable_param, param_id, value)
    end

    def set_non_printable_param_str(param_id, value)
      set_method_str(:set_non_printable_param, param_id, value)
    end

    def set_non_printable_param_datetime(param_id, value)
      set_method_datetime(:set_non_printable_param, param_id, value)
    end

    def set_non_printable_param_bytearray(param_id, value)
      set_method_bytearray(:set_non_printable_param, param_id, value)
    end

    def set_non_printable_param(param_id, value)
      set_method(:set_non_printable_param, param_id, value)
    end

    def get_param_bool(param_id)
      LibFptr.get_param_bool(@interface, param_id) != 0
    end

    def get_param_int(param_id)
      LibFptr.get_param_int(@interface, param_id)
    end

    def get_param_double(param_id)
      LibFptr.get_param_double(@interface, param_id)
    end

    def get_param_str(param_id)
      ptr = FFI::MemoryPointer.new(:pointer, LibFptr::DEFAULT_BUFF_SIZE)
      size = LibFptr.get_param_str(@interface, param_id, ptr, ptr.size)

      if size > ptr.size
        ptr = FFI::MemoryPointer.new(:pointer, size)
        LibFptr.get_param_str(@interface, param_id, ptr, size)
      end

      LibC.wchar_pointer_to_string(ptr)
    end

    def get_param_datetime(param_id)
      year = FFI::MemoryPointer.new(:int)
      month = FFI::MemoryPointer.new(:int)
      day = FFI::MemoryPointer.new(:int)
      hour = FFI::MemoryPointer.new(:int)
      min = FFI::MemoryPointer.new(:int)
      sec = FFI::MemoryPointer.new(:int)

      LibFptr.get_param_datetime(@interface, param_id, year, month, day, hour, min, sec)

      Time.new(year.read_int, month.read_int, day.read_int, hour.read_int, min.read_int, sec.read_int)
    end

    def get_param_bytearray(param_id)
      ptr = FFI::MemoryPointer.new(:uchar, LibFptr::DEFAULT_BUFF_SIZE)
      size = LibFptr.get_param_bytearray(@interface, param_id, ptr, ptr.size)

      if size > ptr.size
        ptr = FFI::MemoryPointer.new(:uchar, size)
        LibFptr.get_param_bytearray(@interface, param_id, ptr, size)
      end

      ptr.get_array_of_uchar(0, size)
    end

    def reset_params
      LibFptr.reset_params(@interface)
    end

    def run_command
      LibFptr.run_command(@interface)
    end

    def beep
      LibFptr.beep(@interface)
    end

    def open_drawer
      LibFptr.open_drawer(@interface)
    end

    def cut
      LibFptr.cut(@interface)
    end

    def device_poweroff
      LibFptr.device_poweroff(@interface)
    end

    def device_reboot
      LibFptr.device_reboot(@interface)
    end

    def open_shift
      LibFptr.open_shift(@interface)
    end

    def reset_summary
      LibFptr.reset_summary(@interface)
    end

    def init_device
      LibFptr.init_device(@interface)
    end

    def query_data
      LibFptr.query_data(@interface)
    end

    def cash_income
      LibFptr.cash_income(@interface)
    end

    def cash_outcome
      LibFptr.cash_outcome(@interface)
    end

    def open_receipt
      LibFptr.open_receipt(@interface)
    end

    def cancel_receipt
      LibFptr.cancel_receipt(@interface)
    end

    def close_receipt
      LibFptr.close_receipt(@interface)
    end

    def check_document_closed
      LibFptr.check_document_closed(@interface)
    end

    def receipt_total
      LibFptr.receipt_total(@interface)
    end

    def receipt_tax
      LibFptr.receipt_tax(@interface)
    end

    def registration
      LibFptr.registration(@interface)
    end

    def payment
      LibFptr.payment(@interface)
    end

    def report
      LibFptr.report(@interface)
    end

    def print_text
      LibFptr.print_text(@interface)
    end

    def print_cliche
      LibFptr.print_cliche(@interface)
    end

    def begin_nonfiscal_document
      LibFptr.begin_nonfiscal_document(@interface)
    end

    def end_nonfiscal_document
      LibFptr.end_nonfiscal_document(@interface)
    end

    def print_barcode
      LibFptr.print_barcode(@interface)
    end

    def print_picture
      LibFptr.print_picture(@interface)
    end

    def print_picture_by_number
      LibFptr.print_picture_by_number(@interface)
    end

    def upload_picture_from_file
      LibFptr.upload_picture_from_file(@interface)
    end

    def download_picture
      LibFptr.download_picture(@interface)
    end

    def clear_pictures
      LibFptr.clear_pictures(@interface)
    end

    def write_device_setting_raw
      LibFptr.write_device_setting_raw(@interface)
    end

    def read_device_setting_raw
      LibFptr.read_device_setting_raw(@interface)
    end

    def commit_settings
      LibFptr.commit_settings(@interface)
    end

    def init_settings
      LibFptr.init_settings(@interface)
    end

    def reset_settings
      LibFptr.reset_settings(@interface)
    end

    def write_date_time
      LibFptr.write_date_time(@interface)
    end

    def write_license
      LibFptr.write_license(@interface)
    end

    def fn_operation
      LibFptr.fn_operation(@interface)
    end

    def fn_query_data
      LibFptr.fn_query_data(@interface)
    end

    def fn_write_attributes
      LibFptr.fn_write_attributes(@interface)
    end

    def external_device_power_on
      LibFptr.external_device_power_on(@interface)
    end

    def external_device_power_off
      LibFptr.external_device_power_off(@interface)
    end

    def external_device_write_data
      LibFptr.external_device_write_data(@interface)
    end

    def external_device_read_data
      LibFptr.external_device_read_data(@interface)
    end

    def operator_login
      LibFptr.operator_login(@interface)
    end

    def process_json
      LibFptr.process_json(@interface)
    end

    def read_device_setting
      LibFptr.read_device_setting(@interface)
    end

    def write_device_setting
      LibFptr.write_device_setting(@interface)
    end

    def begin_read_records
      LibFptr.begin_read_records(@interface)
    end

    def read_next_record
      LibFptr.read_next_record(@interface)
    end

    def end_read_records
      LibFptr.end_read_records(@interface)
    end

    def user_memory_operation
      LibFptr.user_memory_operation(@interface)
    end

    def continue_print
      LibFptr.continue_print(@interface)
    end

    def init_mgm
      LibFptr.init_mgm(@interface)
    end

    def util_form_tlv
      LibFptr.util_form_tlv(@interface)
    end

    def util_mapping
      LibFptr.util_mapping(@interface)
    end

    def util_form_nomenclature
      LibFptr.util_form_nomenclature(@interface)
    end

    def log_write(tag, level, message)
      LibFptr.log_write(LibC.string_to_wchar_pointer(tag), level, LibC.string_to_wchar_pointer(message))
    end

    def show_properties(parent_type, parent)
      LibFptr.show_properties(@interface, parent_type, parent)
    end

    def read_model_flags
      LibFptr.read_model_flags(@interface)
    end

    def line_feed
      LibFptr.line_feed(@interface)
    end

    def flash_firmware
      LibFptr.flash_firmware(@interface)
    end

    def soft_lock_init
      LibFptr.soft_lock_init(@interface)
    end

    def soft_lock_query_session_code
      LibFptr.soft_lock_query_session_code(@interface)
    end

    def soft_lock_validate
      LibFptr.soft_lock_validate(@interface)
    end

    def util_calc_tax
      LibFptr.util_calc_tax(@interface)
    end

    def util_tag_info
      LibFptr.util_tag_info(@interface)
    end

    def bluetooth_remove_paired_devices
      LibFptr.bluetooth_remove_paired_devices(@interface)
    end

    def util_container_versions
      LibFptr.util_container_versions(@interface)
    end

    def activate_licenses
      LibFptr.activate_licenses(@interface)
    end

    def remove_licenses
      LibFptr.remove_licenses(@interface)
    end

    def enter_keys
      LibFptr.enter_keys(@interface)
    end

    def validate_keys
      LibFptr.validate_keys(@interface)
    end

    def enter_serial_number
      LibFptr.enter_serial_number(@interface)
    end

    def get_serial_number_request
      LibFptr.get_serial_number_request(@interface)
    end

    def upload_pixel_buffer
      LibFptr.upload_pixel_buffer(@interface)
    end

    def download_pixel_buffer
      LibFptr.download_pixel_buffer(@interface)
    end

    def print_pixel_buffer
      LibFptr.print_pixel_buffer(@interface)
    end

    def util_convert_tag_value
      LibFptr.util_convert_tag_value(@interface)
    end

    def parse_marking_code
      LibFptr.parse_marking_code(@interface)
    end

    def call_script
      LibFptr.call_script(@interface)
    end

    def set_header_lines
      LibFptr.set_header_lines(@interface)
    end

    def set_footer_lines
      LibFptr.set_footer_lines(@interface)
    end

    def upload_picture_cliche
      LibFptr.upload_picture_cliche(@interface)
    end

    def upload_picture_memory
      LibFptr.upload_picture_memory(@interface)
    end

    def upload_pixel_buffer_cliche
      LibFptr.upload_pixel_buffer_cliche(@interface)
    end

    def upload_pixel_buffer_memory
      LibFptr.upload_pixel_buffer_memory(@interface)
    end

    def exec_driver_script
      LibFptr.exec_driver_script(@interface)
    end

    def upload_driver_script
      LibFptr.upload_driver_script(@interface)
    end

    def exec_driver_script_by_id
      LibFptr.exec_driver_script_by_id(@interface)
    end

    def write_universal_counters_settings
      LibFptr.write_universal_counters_settings(@interface)
    end

    def read_universal_counters_settings
      LibFptr.read_universal_counters_settings(@interface)
    end

    def query_universal_counters_state
      LibFptr.query_universal_counters_state(@interface)
    end

    def reset_universal_counters
      LibFptr.reset_universal_counters(@interface)
    end

    def cache_universal_counters
      LibFptr.cache_universal_counters(@interface)
    end

    def read_universal_counter_sum
      LibFptr.read_universal_counter_sum(@interface)
    end

    def read_universal_counter_quantity
      LibFptr.read_universal_counter_quantity(@interface)
    end

    def clear_universal_counters_cache
      LibFptr.clear_universal_counters_cache(@interface)
    end

    private_class_method def self.finalize(pointer)
      interface_pointer = FFI::MemoryPointer.new(:pointer)
      interface_pointer.write_pointer(pointer)
      LibFptr.destroy(interface_pointer)
    end

    private

    def set_method_bool(method_prefix, param_id, value)
      LibFptr.send("#{method_prefix}_bool", @interface, param_id, value ? 1 : 0)
    end

    def set_method_int(method_prefix, param_id, value)
      LibFptr.send("#{method_prefix}_int", @interface, param_id, value)
    end

    def set_method_double(method_prefix, param_id, value)
      LibFptr.send("#{method_prefix}_double", @interface, param_id, value)
    end

    def set_method_str(method_prefix, param_id, value)
      LibFptr.send("#{method_prefix}_str", @interface, param_id, LibC.string_to_wchar_pointer(value))
    end

    def set_method_datetime(method_prefix, param_id, value)
      year = value.year
      month = value.month
      day = value.day
      hour = value.hour
      min = value.min
      sec = value.sec

      LibFptr.send("#{method_prefix}_datetime", @interface, param_id, year, month, day, hour, min, sec)
    end

    def set_method_bytearray(method_prefix, param_id, value)
      ptr = FFI::MemoryPointer.new(:uchar, (value.empty? ? 0 : value.first.size) * value.length)
      ptr.put_array_of_uchar(0, value)

      LibFptr.send("#{method_prefix}_bytearray", @interface, param_id, ptr, value.size)
    end

    # rubocop:disable CyclomaticComplexity
    def set_method(method_prefix, param_id, value)
      case value
      when Integer
        set_method_int(method_prefix, param_id, value)
      when TrueClass, FalseClass
        set_method_bool(method_prefix, param_id, value)
      when Float, BigDecimal
        set_method_double(method_prefix, param_id, value)
      when Array
        set_method_bytearray(method_prefix, param_id, value)
      when Time, DateTime
        set_method_datetime(method_prefix, param_id, value)
      when String
        set_method_str(method_prefix, param_id, value)
      else
        raise TypeError, "Invalid 'param' type #{value.class}"
      end
    end
    # rubocop:enable CyclomaticComplexity
  end
end
