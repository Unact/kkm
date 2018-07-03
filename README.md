# Kkm

Библиотека для работы с ККМ АТОЛ

Работает только под Linux  
Для разработки на других ОС для гема создан [Dockerfile](https://github.com/Unact/kkm/blob/master/Dockerfile)

В геме реализованы не все методы драйвера АТОЛ  
Методы АТОЛ реализованы через [`C ruby`](https://github.com/Unact/kkm/blob/master/ext/kkm/kkm.cpp)

## Docker

Сборка: `docker build --no-cache -t kkm .`  
Запуск: `docker run -i -t --rm -v "$(pwd)":/app -w /app kkm`

Далее для локальной работы внутри Докера, необходимо скомпилировать исходники командой: `rake compile`  
После этого можно работать с гемом командой: `irb -Ilib -rkkm`

## Работа с гемом

Для работы с устройством ККМ необходимо создать экземпляр [`Kkm::Device`](https://github.com/Unact/kkm/blob/master/lib/kkm/device.rb)  
При этом будет автоматически включен ККМ и по завершению выключен  
В результате выполнения ККМ(по настройкам соединения в `./spec/device/kkm.yaml`) должен издать звук и распечатать текст "OMG"

```ruby
  settings = YAML.load_file(File.open('./spec/device/kkm.yaml'))
  device = Kkm::Device.new(settings)
  device.work do |device_interface|
    device_interface.beep
    device_interface.print_text("OMG")
  end
```

Если нужен контроль над включением/выключением ККМ, то необходимо создать экземпляр [`Kkm::DeviceInterface`](https://github.com/Unact/kkm/blob/master/lib/kkm/device_interface.rb)  
Результат должен быть таким же, как и пример выше

```ruby
  settings = YAML.load_file(File.open('./spec/device/kkm.yaml'))
  device_interface = Kkm::DeviceInterface.new(settings)
  device_interface.turn_on
  device_interface.beep
  device_interface.print_text("OMG")
  device_interface.turn_off
```

## Доработка гема

Все новые методы(методы которые состоят из нескольких методов АТОЛ), следует реализовывать на ruby, в классе [`Kkm::DeviceInterface`](https://github.com/Unact/kkm/blob/master/lib/kkm/device_interface.rb)

Все доработки методов из [`ifptr.h`](https://github.com/Unact/kkm/blob/master/ext/kkm/lib/include/ifptr.h) стоит производить в основном `.cpp` файле гема
