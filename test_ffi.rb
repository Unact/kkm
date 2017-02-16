require 'rubygems'
require 'ffi'

module FFIFptr
  extend FFI::Library

  ffi_lib "./bins/linux-x64/libfptr.so"

  attach_function(:CreateFptrInterface, [:int], :pointer)
  attach_function(:ReleaseFptrInterface, [:pointer], :int)
end

module FFIKkm
  extend FFI::Library

  ffi_lib "./bins/linux-x64/libKkm.so"
  
  attach_function(:put_DeviceSingleSetting, [:pointer, :string, :string], :int)
  attach_function(:put_DeviceSettings, [:pointer, :string], :int)
  attach_function(:get_DeviceSettings, [:pointer, :pointer, :int], :int)
  attach_function(:checkError, [:pointer], :void)
  attach_function(:ApplySingleSettings, [:pointer], :int)
  attach_function(:printFooter, [:pointer], :void)
  attach_function(:put_DeviceEnabled, [:pointer], :int)
end

ifptr = FFIFptr.CreateFptrInterface(11)
p ifptr

res = FFIKkm.put_DeviceSingleSetting(ifptr, "AccessPassword".force_encoding("UTF-16LE"), "0".force_encoding("UTF-16LE"))
res = FFIKkm.put_DeviceSingleSetting(ifptr, "BaudRate", "9600")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "Bits", "8")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "ConnectionType", "1")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "IPAddress", "192.168.101.64")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "IPPort", "5555")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "Model", "63")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "OfdPort", "NONE")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "Parity", "0")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "Port", "TCPIP")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "Protocol", "0")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "StopBits", "0")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "TTYSuffix", "ttyACM0")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "UserPassword", "30")
res = FFIKkm.put_DeviceSingleSetting(ifptr, "ttyACM0", "63")
res = FFIKkm.ApplySingleSettings(ifptr)
p res
# FFIKkm.checkError(ifptr)


buf = FFI::MemoryPointer.from_string(' '*2000)
res = FFIKkm.get_DeviceSettings(ifptr, buf, buf.size)
res = buf.read_string(2000)
puts res.strip
p ifptr
res = FFIKkm.put_DeviceEnabled(ifptr)
res = FFIKkm.printFooter(ifptr)

puts "Ok"

