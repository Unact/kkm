# frozen_string_literal: true

require "ffi"

module KKM
  # Ruby FFI utility methods for working with KKM::Libfptr
  module LibC
    extend FFI::Library
    ffi_lib FFI::Library::LIBC

    attach_function :mbstowcs, [:pointer, :pointer, :size_t], :size_t
    attach_function :wcstombs, [:pointer, :pointer, :size_t], :size_t

    def self.wchar_pointer_to_string(wchar_pointer)
      size = wcstombs(nil, wchar_pointer, 0)
      str_pointer = FFI::MemoryPointer.new(:pointer, size)

      wcstombs(str_pointer, wchar_pointer, size)

      str_pointer.read_string.force_encoding(Encoding::UTF_8)
    end

    def self.string_to_wchar_pointer(value)
      size = mbstowcs(nil, value, 0)
      wchar_pointer = FFI::MemoryPointer.new(:pointer, size)

      mbstowcs(wchar_pointer, value, size)

      wchar_pointer
    end
  end
end
