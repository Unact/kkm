# Kkm

Библиотека для работы с ККМ АТОЛ.

Работает только под Linux.

Для работы на других ОС для гема создан [Dockerfile](https://github.com/Unact/kkm/blob/master/Dockerfile).

В геме реализованы не все методы драйвера АТОЛ

Методы АТОЛ реализованы через [`C ruby`](https://github.com/Unact/kkm/blob/master/ext/kkm/kkm.cpp)

## Docker

Сборка: `docker build --no-cache -t kkm .`

Запуск: `docker run -i -t --rm -v "$(pwd)":/app -w /app kkm`

Далее для локальной работы внутри Докера, необходимо скомпилировать исходники
командой: `rake compile`

После этого можно работать с гемом командой: `irb -Ilib -rkkm`

## Работа с гемом

Для работы с устройством Ккм необходимо создать экземпляр `Kkm::DeviceInterface`

Пример:
```ruby
  settings = YAML.load_file File.open('./spec/device/kkm.yaml')
  device = Kkm::DeviceInterface.new settings
  device.beep
  device.print_text "OMG"
```
Должен издать звук и распечатать текст "OMG"

## Доработка гема
При доработке гема, все составные методы(методы которые состоят из нескольких методов АТОЛ),
следует реализовывать на ruby, в классе `Kkm::DeviceInterface`

Все доработки методов из [`ifptr.h`](https://github.com/Unact/kkm/blob/master/ext/kkm/lib/include/ifptr.h)
стоит производить в основном `.cpp` файле гема
