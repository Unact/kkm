# KKM

Библиотека на руби для работы с ККТ/ККМ [АТОЛ](https://integration.atol.ru/)  
Полная имплементация ДТО 10 с использованием Ruby FFI  
Имплементирует интерфейс[ДТО](https://github.com/Unact/kkm/blob/master/support/libfptr10.h)  

**Требуется предварительно установленный ДТО 10**  

## Работа с гемом

В геме полностью имплементированы методы нативного драйвер ККТ на C.  
Для работы на прямую используется [`KKM::LibFptr`](https://github.com/Unact/kkm/blob/master/lib/kkm/libfptr.rb)  
При таком использовании, вся ответственность о работе с памятью возлагается на создаваемое решение.  
Если не хочется вручную работать с памятью, то следует использовать [`KKM::IFptr`](https://github.com/Unact/kkm/blob/master/lib/kkm/ifptr.rb)  

### Примеры

#### Общее

В результате выполнения ККТ(по настройкам соединения) должен распечатать текст "OMG"

```ruby
  require "json"

  settings = { "IPAddress" => "0.0.0.0", "IPPort" => "5555", "Model" => "63", "Port" => "2" }.to_json
  fptr = IFptr.new
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.set_settings(settings) != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.open != KKM::LibFptr::LIBFPTR_OK

  fptr.set_param_str(KKM::LibFptr::LIBFPTR_PARAM_TEXT, "OMG")
  fptr.set_param_int(KKM::LibFptr::LIBFPTR_PARAM_ALIGNMENT, KKM::LibFptr::LIBFPTR_ALIGNMENT_CENTER)
  fptr.set_param_int(KKM::LibFptr::LIBFPTR_PARAM_TEXT_WRAP, KKM::LibFptr::LIBFPTR_TW_WORDS)

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.print_text != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.close != KKM::LibFptr::LIBFPTR_OK
```

#### Закрытие смены

```ruby
  require "json"

  settings = { "IPAddress" => "0.0.0.0", "IPPort" => "5555", "Model" => "63", "Port" => "2" }.to_json
  fptr = IFptr.new
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.set_settings(settings) != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.open != KKM::LibFptr::LIBFPTR_OK

  fptr.set_param_str(1021, "Тест")
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.operator_login != KKM::LibFptr::LIBFPTR_OK

  fptr.set_param_bool(KKM::LibFptr::LIBFPTR_PARAM_REPORT_ELECTRONICALLY, true)
  fptr.set_param_int(KKM::LibFptr::LIBFPTR_PARAM_REPORT_TYPE, KKM::LibFptr::LIBFPTR_RT_CLOSE_SHIFT)
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.report != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.close != KKM::LibFptr::LIBFPTR_OK
```

#### Печать чека

```ruby
  require "json"

  settings = { "IPAddress" => "0.0.0.0", "IPPort" => "5555", "Model" => "63", "Port" => "2" }.to_json
  fptr = IFptr.new
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.set_settings(settings) != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.open != KKM::LibFptr::LIBFPTR_OK

  fptr.set_param_str(1021, "Тест")
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.operator_login != KKM::LibFptr::LIBFPTR_OK

  fptr.set_param_str(1008, "example@example.com")
  fptr.set_param_int(KKM::LibFptr::LIBFPTR_PARAM_RECEIPT_TYPE, KKM::LibFptr::LIBFPTR_RT_SELL)
  fptr.set_param_bool(KKM::LibFptr::LIBFPTR_PARAM_RECEIPT_ELECTRONICALLY, true)
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.open_receipt != KKM::LibFptr::LIBFPTR_OK

  fptr.set_param_str(KKM::LibFptr::LIBFPTR_PARAM_COMMODITY_NAME, "Тест")
  fptr.set_param_double(KKM::LibFptr::LIBFPTR_PARAM_PRICE, 10)
  fptr.set_param_double(KKM::LibFptr::LIBFPTR_PARAM_QUANTITY, 1)
  fptr.set_param_int(KKM::LibFptr::LIBFPTR_PARAM_TAX_TYPE, KKM::LibFptr::LIBFPTR_TAX_NO)
  fptr.set_param_int(1212, 1)
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.registration != KKM::LibFptr::LIBFPTR_OK
  
  fptr.set_param_double(KKM::LibFptr::LIBFPTR_PARAM_PAYMENT_SUM, 10)
  fptr.set_param_int(KKM::LibFptr::LIBFPTR_PARAM_PAYMENT_TYPE, KKM::LibFptr::LIBFPTR_PT_CASH)
  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.payment != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.close_receipt != KKM::LibFptr::LIBFPTR_OK

  raise "#{fptr.error_code} [#{fptr.error_description}]" if fptr.close != KKM::LibFptr::LIBFPTR_OK
```

## Доработка гема

При обновлении версии ДТО, все новые методы добавляемые ДТО, необходимо добавить в классы `KKM::LibFptr`, `KKM::IFptr`
