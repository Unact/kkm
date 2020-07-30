# KKM

Библиотека на руби для работы с ККТ/ККМ АТОЛ  
Полная имплементация ДТО 10 с использованием Ruby FFI  
Текущая имплементированная версия [ДТО](https://github.com/Unact/kkm/blob/master/lib/kkm/version.rb#L5)

**Поддерживает только Linux x64, Windows x64, MacOS x86_64**  
Для использования библиотеки на других архитектурах, необходимо иметь установленной библиотеку с названием `fptr10` или `libfptr10`

## Работа с гемом

В геме полностью имплементированы методы нативного драйвер ККТ на C.  
Для работы на прямую используется [`KKM::LibFptr`](https://github.com/Unact/kkm/blob/master/lib/kkm/libfptr.rb)  
При таком использовании, вся ответственность о работе с памятью возлагается на создаваемое решение.  
Если не хочется вручную работать с памятью, то следует использовать [`KKM::IFptr`](https://github.com/Unact/kkm/blob/master/lib/kkm/ifptr.rb)  

Также в геме присутсвует возможность работа с ККТ в упрощенном формате, для этого необходимо создать экземпляр [`KKM::Device`](https://github.com/Unact/kkm/blob/master/lib/kkm/device.rb)  

### Примеры

#### Общее

В результате выполнения ККТ(по настройкам соединения) должен распечатать текст "OMG"
При этом будет автоматически включен ККТ и по завершению выключен  

```ruby
  settings = KKM::Models::Settings.new("IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2")
  device = KKM::Device.new(settings)
  text_line = KKM::Models::TextLine.new("OMG")

  device.work do
    device.print_text_line(text_line)
  end
```

Если нужен контроль над включением/выключением ККТ, то следует использовать методы `turn_on`/`turn_off`
Результат должен быть таким же, как и пример выше

```ruby
  settings = KKM::Models::Settings.new("IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2")
  device = KKM::Device.new(settings)
  text_line = KKM::Models::TextLine.new("OMG")

  device.turn_on
  device.print_text_line(text_line)
  device.turn_off
```

#### Закрытие смены

```ruby
  settings = KKM::Models::Settings.new("IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2")
  device = KKM::Device.new(settings)
  operator = KKM::Models::Operator.new('Тест')

  device.work do
    device.close_day(operator, electronically: true)
  end
```

#### Печать отчета

```ruby
  settings = KKM::Models::Settings.new("IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2")
  device = KKM::Device.new(settings)
  report = KKM::Models::Report.new(KKM::LibFptr::LIBFPTR_RT_X)

  device.work do
    device.print_report(report)
  end
```

#### Печать чека

```ruby
  settings = KKM::Models::Settings.new("IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2")
  device = KKM::Device.new(settings)
  receipt = KKM::Models::Receipt.new
  receipt.type = KKM::LibFptr::LIBFPTR_RT_SELL
  receipt.positions = [
    KKM::Models::Receipt::Position.new(
      'Тест',
      10,
      1,
      KKM::LibFptr::LIBFPTR_TAX_NO,
      [KKM::Models::Tag.new(KKM::Constants::Tag::POSITION_TYPE, KKM::Constants::PositionType::GOODS)]
    )
  ]
  receipt.payments = [KKM::Models::Receipt::Payment.new(10, KKM::LibFptr::LIBFPTR_PT_CASH)]
  receipt.operator = KKM::Models::Operator.new('Тест')
  receipt.tags = [KKM::Models::Tag.new(KKM::Constants::Tag::BUYER_ADDRESS, 'example@example.com')]
  
  device.work do
    device.print_receipt(receipt, electronically: true)
  end
```

## Доработка гема

Все новые методы, следует реализовывать в классе [`KKM::Device`](https://github.com/Unact/kkm/blob/master/lib/kkm/device.rb)  
При обновлении версии ДТО, все новые методы добавляемые ДТО, следует добавлять в классы `KKM::LibFptr`, `KKM::IFptr`, `KKM::Device`
