require 'rubygems'
require 'ffi'

module FFIFptr
  extend FFI::Library

  ffi_lib "./cpp_so/bins/linux-x64/libfptr.so"

  attach_function(:CreateFptrInterface, [:int], :pointer)
  attach_function(:ReleaseFptrInterface, [:pointer], :int)
end

module FFIKkm
  extend FFI::Library

  ffi_lib "./cpp_so/build/libkkm_interface.so"
  enum :Alignment, [:AlignmentLeft, 0,
              :AlignmentCenter,
              :AlignmentRight
  ]
  enum :TextWrap, [ :TextWrapNone, 0, 
                   :TextWrapWord,        
                   :TextWrapLine
  ]
  # Типы / состояния чека
  enum :ChequeType, [
    :ChequeClosed, 0,            # Чек закрыт                               
    :ChequeSell,                 # Чек продажи / прихода                    
    :ChequeSellReturn,           # Чек возврат продажи / прихода            
    :ChequeSellAnnulate,         # Чек аннулирования продажи                
    :ChequeBuy,                  # Чек покупки / расхода                    
    :ChequeBuyReturn,            # Чек возврата покупки / расхода           
    :ChequeBuyAnnulate,          # Чек аннулирования покупки                
    :ChequeSellCorrection,       # Чек коррекции прихода                    
    :ChequeSellReturnCorrection, # Чек коррекции возврата прихода           
    :ChequeBuyCorrection,        # Чек коррекции расхода                    
    :ChequeBuyReturnCorrection,  # Чек коррекции возврата расхода           
  ]
  attach_function(:put_DeviceSingleSetting, [:pointer, :string, :string], :int)
  attach_function(:put_DeviceSettings, [:pointer, :string], :int)
  attach_function(:get_DeviceSettings, [:pointer, :pointer, :int], :int)
  attach_function(:checkError, [:pointer], :void)
  attach_function(:ApplySingleSettings, [:pointer], :int)
  attach_function(:printFooter, [:pointer], :void)
  attach_function(:put_DeviceEnabled, [:pointer, :int], :int)
  attach_function(:GetStatus, [:pointer], :int)
  attach_function(:CancelCheck, [:pointer], :int)
  attach_function(:printText,[:pointer, :string, :Alignment, :TextWrap], :void)
  attach_function(:openCheck,[:pointer, :ChequeType], :void)
  attach_function(:closeCheck,[:pointer, :int], :void)
  attach_function(:registrationFZ54, [:pointer, :string, :double, :double, :int], :void)
  attach_function(:payment, [:pointer, :double, :int], :void)
end

ifptr = FFIFptr.CreateFptrInterface(11)
p ifptr

res = FFIKkm.put_DeviceSettings(ifptr, '
<?xml version="1.0" encoding="UTF-8"?>
<settings version="5">
    <value name="AccessPassword">0</value>
    <value name="AutoDisableBluetooth">0</value>
    <value name="AutoEnableBluetooth">1</value>
    <value name="BaudRate">9600</value>
    <value name="Bits">8</value>
    <value name="ConnectionType">1</value>
    <value name="DeviceName">Device #1</value>
    <value name="IPAddress">192.168.101.64</value>
    <value name="IPPort">5555</value>
    <value name="MACAddress"></value>
    <value name="Model">63</value>
    <value name="OfdPort">NONE</value>
    <value name="Parity">0</value>
    <value name="Port">TCPIP</value>
    <value name="Protocol">0</value>
    <value name="StopBits">0</value>
    <value name="TTYSuffix">ttyACM0</value>
    <value name="UserPassword">30</value>
    <value name="ttyACM0">63</value>
</settings>')

buf = FFI::MemoryPointer.from_string( ' ' * 2000 )


res = FFIKkm.get_DeviceSettings(ifptr, buf, buf.size)
res = buf.read_string(2000)
puts res.strip
res = FFIKkm.put_DeviceEnabled(ifptr,1)
res = FFIKkm.GetStatus(ifptr)
res = FFIKkm.CancelCheck(ifptr)
# FFIKkm.printText(ifptr,"Здесь был Вова!", :AlignmentLeft, :TextWrapWord)
# FFIKkm.printFooter(ifptr)

FFIKkm.openCheck(ifptr, :ChequeSell)


FFIKkm.registrationFZ54(ifptr, "Позиция KuKu", 1.2, 66.0, 3)
FFIKkm.registrationFZ54(ifptr, "Шпроты в масле", 1.13, 2.0, 3)
FFIKkm.payment(ifptr, (1.2*66.0 + 1.13*2.0), 0)

FFIKkm.closeCheck(ifptr, 0)

puts "Ok"

