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
  
  # Режимы ККТ
  enum :Mode, [
    :ModeSelect, 0,         # Выбора                                   
    :ModeRegistration,       # Регистрации                              
    :ModeReportNoClear,      # Отчетов без гашения                      
    :ModeReportClear,        # Отчетов с гашением                       
    :ModeProgramming,        # Программирования                         
    :ModeFiscalMemory,       # Доступа к фискальной памяти              
    :ModeEKLZ,               # Доступа к ЭКЛЗ                           
    :ModeExtra               # Дополнительный                           
  ]
  # Типы отчетов
  enum :ReportType, [
    :ReportCRCLear, 0,             # Гашение контрольной ленты            
    :ReportZ, 1,                    # Отчет с гашением                     
    :ReportX, 2,                    # Отчет без гашения                    
    :ReportFiscalDatesReduced, 3,   # Сокращенный фискальный отчет по диапазону дат                  
    :ReportFiscalSessionsReduced, 4, # Сокращенный фискальный отчет по диапазону смен                
    :ReportFiscalDatesFull, 5,      # Полный фискальный отчет по диапазону дат                  
    :ReportFiscalSessionsFull, 6,   # Полный фискальный отчет по диапазону смен                 
    :ReportDepartment, 7,           # Отчет по секциям                     
    :ReportCashiers, 8,             # Отчет по кассирам                    
    :ReportHours, 10,               # Отчет по часам                       
    :ReportQuantity, 11,            # Отчет по кол-ву                      
    :ReportRibbon, 12,              # Данные КЛ                            
    :ReportRibbonCheque, 13,        # Данные КЛ по чеку                    
    :ReportDumpRibbon, 14,          # Дамп КЛ                              
    :ReportDumpRibbonCheque, 15,    # Дамп КЛ по чеку                      
    :ReportRom, 19,                 # ПО ККТ                               
    :ReportRomUnit, 20,             # ПО модуля ККТ                        
    :ReportEKLZActivationTotal, 22, # ЭКЛЗ итоги активации                 
    :ReportEKLZSessionTotal, 23,    # ЭКЛЗ итоги смены                     
    :ReportEKLZSessionCR, 24,       # ЭКЛЗ КЛ смены                        
    :ReportEKLZKPKDoc, 25,          # ЭКЛЗ документ по номеру              
    :ReportEKLZDatesDepartmentsReduced, 26, # ЭКЛЗ по датам краткий по секциям            
    :ReportEKLZDatesDepartmentsFull, 27,    # ЭКЛЗ по датам полный по секциям            
    :ReportEKLZDatesSessionsTotalReduced, 28,   # ЭКЛЗ по датам краткий по итогам смен        
    :ReportEKLZDatesSessionsTotalFull, 29,  # ЭКЛЗ по датам полный по итогам смен        
    :ReportEKLZSessionsDepartmentsReduced, 30,  # ЭКЛЗ по сменам краткий по секциям            
    :ReportEKLZSessionsDepartmentsFull, 31, # ЭКЛЗ по сменам полный по секциям            
    :ReportEKLZSessionsTotalReduced, 32,    # ЭКЛЗ по сменам краткий по итогам смен        
    :ReportEKLZSessionsTotalFull, 33,   # ЭКЛЗ по сменам полный по итогам смен        
    :ReportDocumentByNumber, 34,    # Печать документа по номеру           
    :ReportCRPrintFull, 35,         # Печать КЛ полностью                  
    :ReportCRPrintReduced, 36,      # Печать КЛ сокращенно                 
    :ReportService, 38,             # Служебный отчет                      
    :ReportSD, 39,                  # Электронный отчет с SD               
    :ReportJournalData, 41,         # Данные ЭЖ                            
    :ReportAccountingState, 42,     # Состояние расчетов                   
    :ReportPrintInfo, 43,           # Печать информации о ККТ              
    :ReportTestDevice, 44,          # Тестирование ККТ                     
    :ReportOfdConnectionDiagnostic, 45 # Диагностика соединения с ОФД     
  ]
  
  # Тип штрихкода
enum :BarcodeType, [
    :BarcodeEAN8, 0,        # EAN8                                         
    :BarcodeEAN13,           # EAN13                                        
    :BarcodeUPCA,            # UPCA                                         
    :BarcodeCode39,          # Code 39                                      
    :BarcodeQR,              # QR-код                                       
    :BarcodePDF417,          # PDF417                                       
    :BarcodeITF14,           # ITF-14                                       
    :BarcodeInterleaved2of5, # Interleaved Two of Five                      
    :BarcodeUPCE,            # UPCE                                         
    :BarcodeCodabar,         # CODABAR                                      
    :BarcodeCode93,          # Code 93                                      
    :BarcodeCode128,         # Code 128                                     
]

# Уровень коррекции QR-кода
enum :BarcodeQRCorrection, [
    :BarcodeQRCorrectionDefault, 0, # По умолчанию/выбору ККТ              
    :BarcodeQRCorrectionLevelL,      # Low                                  
    :BarcodeQRCorrectionLevelM,      # Medium                               
    :BarcodeQRCorrectionLevelQ,      # Quartile                             
    :BarcodeQRCorrectionLevelH       # High                                 
]

# Кодировка QR-кода
enum :BarcodeQREncoding, [
    :BarcodeQREncodingISO8859, 0,   # ISO-8859                             
    :BarcodeQREncodingUTF8,          # UTF-8                                
]

# Режим кодировки QR-кода
enum :BarcodeQREncodingMode, [
    :BarcodeQREncodingModeNumbers, 0,      # Числовой                      
    :BarcodeQREncodingModeNumbersLetters,   # Буквенно-числовой             
    :BarcodeQREncodingMode8Bit,             # 8-битный                      
    :BarcodeQREncodingModeECI               # 8-битный ECI                  
]

# Режим упаковки PDF417-кода
enum :BarcodePDF417PackingMode, [ 
    :BarcodePDF417PackingModeDefault, 0,    # Автоматический               
    :BarcodePDF417PackingModeText,           # Текстовый                    
    :BarcodePDF417PackingModeBin,            # Бинарный                     
    :BarcodePDF417PackingModeNumbers         # Числовой                     
]

# Способ печати ШК
enum :BarcodePrintType, [
    :BarcodeSoftware,  0,    # Генерация изображения штрихкода средствами
                             #   драйвера и печать его как картинки            
    :BarcodeHardware,        # Печать ШК средствами устройства              
    :BarcodeAuto             # Автоматический выбор способа печати          
]

  attach_function(:put_DeviceSingleSetting, [:pointer, :string, :string], :int)
  attach_function(:put_DeviceSettings, [:pointer, :string], :int)
  attach_function(:get_DeviceSettings, [:pointer, :pointer, :int], :int)
  attach_function(:checkError, [:pointer], :void)
  attach_function(:ApplySingleSettings, [:pointer], :int)
  attach_function(:put_DeviceEnabled, [:pointer, :int], :int)
  attach_function(:GetStatus, [:pointer], :int)
  attach_function(:CancelCheck, [:pointer], :int)
  
  attach_function(:printFooter, [:pointer], :void)
  attach_function(:printText,[:pointer, :string, :Alignment, :TextWrap], :void)
  attach_function(:openCheck,[:pointer, :ChequeType], :void)
  attach_function(:closeCheck,[:pointer, :int], :void)
  attach_function(:registrationFZ54, [:pointer, :string, :double, :double, :int], :void)
  
  attach_function(:get_ResultCode, [:pointer, :pointer], :int)
  attach_function(:get_ResultDescription, [:pointer, :pointer, :int], :int)
  attach_function(:get_BadParamDescription, [:pointer, :pointer, :int], :int)
  attach_function(:put_Caption, [:pointer, :string], :int)
  attach_function(:put_TextWrap, [:pointer, :int], :int)
  attach_function(:put_Alignment, [:pointer, :int], :int)
  attach_function(:PrintString, [:pointer], :int)
  attach_function(:put_Mode, [:pointer, :int], :int)
  attach_function(:SetMode, [:pointer], :int)
  attach_function(:put_CheckType, [:pointer, :int], :int)
  attach_function(:OpenCheck, [:pointer], :int)
  attach_function(:put_TypeClose, [:pointer, :int], :int)
  attach_function(:CloseCheck, [:pointer], :int)
  attach_function(:put_TaxNumber, [:pointer, :int ], :int)
  attach_function(:put_Quantity, [:pointer, :double], :int)
  attach_function(:put_Price, [:pointer, :double], :int)
  attach_function(:put_Name, [:pointer, :string], :int)
  attach_function(:Registration, [:pointer], :int)
  attach_function(:put_Summ, [:pointer, :double], :int)
  attach_function(:Payment, [:pointer], :int)
  attach_function(:get_Remainder, [:pointer, :pointer], :int)
  attach_function(:get_Change, [:pointer, :pointer], :int)
  attach_function(:put_ReportType, [:pointer, :int], :int)
  attach_function(:Report, [:pointer], :int)
  attach_function(:PrintFooter, [:pointer], :int)
  attach_function(:put_BarcodeType, [:pointer, :int], :int)
  attach_function(:put_Barcode, [:pointer, :string], :int)
  attach_function(:put_Height, [:pointer, :int], :int)
  attach_function(:put_BarcodeVersion, [:pointer, :int], :int)
  attach_function(:put_BarcodePrintType, [:pointer, :int], :int)
  attach_function(:put_PrintBarcodeText, [:pointer, :int], :int)
  attach_function(:put_BarcodeControlCode, [:pointer, :int], :int)
  attach_function(:put_Scale, [:pointer, :double], :int)
  attach_function(:put_BarcodeCorrection, [:pointer, :int], :int)
  attach_function(:put_BarcodeColumns, [:pointer, :int], :int)
  attach_function(:put_BarcodeRows, [:pointer, :int], :int)
  attach_function(:put_BarcodeProportions, [:pointer, :int], :int)
  attach_function(:put_BarcodeUseProportions, [:pointer, :int], :int)
  attach_function(:put_BarcodePackingMode, [:pointer, :int], :int)
  attach_function(:put_BarcodePixelProportions, [:pointer, :int], :int)
  attach_function(:PrintBarcode, [:pointer], :int)
  attach_function(:put_DiscountType, [:pointer, :int], :int)
  attach_function(:put_Destination, [:pointer, :int], :int)
  attach_function(:Discount, [:pointer], :int)
  attach_function(:Charge, [:pointer], :int)
end

def payment(ifptr, sum, type)
    if FFIKkm.put_Summ(ifptr, sum) < 0
        FFIKkm.checkError(ifptr)
    end
    if FFIKkm.put_TypeClose(ifptr, type) < 0
        FFIKkm.checkError(ifptr)
    end
    if FFIKkm.Payment(ifptr) < 0
        FFIKkm.checkError(ifptr)
    end
    p_remainder = FFI::MemoryPointer.new(:double)
    FFIKkm.get_Remainder(ifptr, p_remainder);
    remainder = p_remainder.read_double
    p_change = FFI::MemoryPointer.new(:double)
    FFIKkm.get_Change(ifptr, p_change);
    change = p_change.read_double
    puts "Remainder: #{remainder}, Change: #{change}"                         
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

# FFIKkm.printText(ifptr,"Здесь текст!", :AlignmentLeft, :TextWrapWord)
# FFIKkm.printFooter(ifptr)

FFIKkm.openCheck(ifptr, :ChequeSell)

FFIKkm.registrationFZ54(ifptr, "Позиция KuKu", 1.2, 66.0, 3)
FFIKkm.registrationFZ54(ifptr, "Шпроты в масле", 1.13, 2.0, 3)
payment(ifptr, (1.2*66.0 + 1.13*2.0), 0)

FFIKkm.closeCheck(ifptr, 0)

puts "Ok"

