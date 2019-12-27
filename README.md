# Kkm

Библиотека для работы с ККТ АТОЛ  
Полная имплементация ДТО 10 с использованием Ruby FFI  
Текущая имплементированная версия [ДТО](https://github.com/Unact/kkm/blob/master/lib/kkm/version.rb#5)

**Поддерживает только Linux x64, Windows x64, MacOS x86_64**  
Для использования библиотеки на других архитектурах, необходимо иметь установленной библиотеку с названием `fptr10` или `libfptr10`

## Работа с гемом

Для работы с устройством ККТ необходимо создать экземпляр [`Kkm::Device`](https://github.com/Unact/kkm/blob/master/lib/kkm/device.rb)  
При этом будет автоматически включен ККТ и по завершению выключен  
В результате выполнения ККТ(по настройкам соединения) должен издать звук и распечатать текст "OMG"

```ruby
  settings = { "DeviceName" => "Test", "IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2" }
  device = Kkm::Device.new(settings)
  device.work do
    device.beep
    device.print_text("OMG")
  end
```

Если нужен контроль над включением/выключением ККТ, то следует использовать методы `open`/`close`
Результат должен быть таким же, как и пример выше

```ruby
  settings = { "DeviceName" => "Test", "IPAddress" => "192.168.101.64", "IPPort" => "5555", "Model" => "63", "Port" => "2" }
  device = Kkm::Device.new(settings)
  device.open
  device.beep
  device.print_text("OMG")
  device.close
```

## Доработка гема

Все новые методы, следует реализовывать в классе [`Kkm::Device`](https://github.com/Unact/kkm/blob/master/lib/kkm/device.rb)
