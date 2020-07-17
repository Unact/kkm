## 0.2.6

- Исправлены ошибки работы методов print_receipt и print_slip для платформы 5.0

## 0.2.5

- Обновлена версия драйвера ДТО до 10.7.0.0
- Добавлена константа `Kkm::Constants::FiscalProperty::POSITION_MARKING_CODE` для тега 1162
- Добавлена возможность передавать идентификатор ККТ для разделения логов ККТ

## 0.2.4

- Добавлены константы с суммами по чеку разных типов оплат
- Переименована константа `Kkm::Constants::FiscalProperty::CASH_SUM` в `Kkm::Constants::FiscalProperty::RECEIPT_CASH_PAYMENT_TYPE`

## 0.2.3

- Исправлен метод `Kkm::LibFptr.get_version_string`

## 0.2.2

- Исправлен memory leak связанный с не вызыванием `Kkm::LibFptr.destroy`

## 0.2.1

- Добавлена возможность задавать таймаут при подключении к ККТ. Задается через настройку "Timeout"
- Убраны методы `Kkm::Device.open` и `Kkm::Device.close`. Вместо них добавлены методы `Kkm::Device.turn_on` и `Kkm::Device.turn_off`
- Переименована константа `Kkm::Constants::DEFAULT_TIMEOUT` в `Kkm::Constants::DEFAULT_TIMEOUT_ENQ`

## 0.2.0

**Breaking change** Имплементировать ДТО10, библиотека полностью переписана

- Добавленая имплементация ДТО версии 10.6.1.0
- Добавлена возможность использования библиотеки на macOS, Windows
- Убраны классы `Kkm::DeviceInterface` и `Kkm::DeviceDriver`. Для печати чеков теперь есть только один класс `Kkm::Device`
- На все ошибки ККТ теперь возвращается `Kkm::DeviceError` вместо `Kkm::DeviceInterfaceError`
- Многие методы были либо убраны, либо были переименованы в соотвествии с новым ДТО. Ниже полный список изменений

`Kkm::DeviceInterface.reboot` - `Kkm::Device.device_reboot`
`Kkm::DeviceInterface.turn_on` - `Kkm::Device.open`
`Kkm::DeviceInterface.turn_off` - `Kkm::Device.close`
`Kkm::DeviceInterface.print_cheque_sell` - `Kkm::Device.print_receipt`
`Kkm::DeviceInterface.print_cheque_sell_return` - `Kkm::Device.print_receipt`
`Kkm::DeviceInterface.register_goods` - `Kkm::Device.register_position`
`Kkm::DeviceInterface.pay_for_goods` - `Kkm::Device.register_payment`
`Kkm::DeviceInterface.last_check_info`  - `Kkm::Device.fn_last_receipt_data`
`Kkm::DeviceInterface.last_check_fiscal_doc_number`  - `Kkm::Device.fn_last_receipt_data`
`Kkm::DeviceInterface.last_check_sum`  - `Kkm::Device.fn_last_receipt_data`
`Kkm::DeviceInterface.last_check_datetime`  - `Kkm::Device.fn_last_receipt_data`
`Kkm::DeviceInterface.last_check_doc_number`  - `Kkm::Device.fn_last_receipt_data`
`Kkm::DeviceInterface.last_check_type`  - `Kkm::Device.fn_last_receipt_data`
`Kkm::DeviceInterface.current_close_session` - `Kkm::Device.shift_state_data`
`Kkm::DeviceInterface.last_check_session_number` - `Kkm::Device.status_data`
`Kkm::DeviceInterface.last_check_session_check_number` - `Kkm::Device.status_data`
`Kkm::DeviceInterface.current_mode` - `Kkm::Device.mode_raw_data`
`Kkm::DeviceInterface.fn_status` - `Kkm::Device.fn_status_raw_data`
`Kkm::DeviceInterface.kkm_status` - `Kkm::Device.status_raw_data`
`Kkm::DeviceInterface.ofd_status` - `Kkm::Device.ofd_status_raw_data`
`Kkm::DeviceInterface.last_check_web_params` - убран
`Kkm::DeviceInterface.read_kkm_fiscal_property` - убран

- Убраны следующие константы, так как они появились в новом ДТО, теперь необходимо использовать эти константы
`Kkm::Constants::ReportType`
`Kkm::Constants::Mode`
`Kkm::Constants::TextWrap`
`Kkm::Constants::FFDVersion`
`Kkm::Constants::Alignment`
`Kkm::Constants::ChequeType`
`Kkm::Constants::FiscalPropertyType`
`Kkm::Constants::ChequeTypeNumber`
`Kkm::Constants::RegisterNumber`
`Kkm::Constants::Tax`
`Kkm::Constants::TaxNumber`
`Kkm::Constants::AgentType`
`Kkm::Constants::Command::FN_CHEQUE_DOCUMENT_DATA`
`Kkm::Constants::Command::FN_CHEQUE_DOCUMENT`

- Переименованы следующие константы
`Kkm::Constants::FiscalProperty::CHEQUE_TYPE` => `Kkm::Constants::FiscalProperty::RECEIPT_TYPE`
`Kkm::Constants::FiscalProperty::CHEQUE_NUMBER` => `Kkm::Constants::FiscalProperty::RECEIPT_NUMBER`
`Kkm::Constants::FiscalProperty::PAYMENT_ADDRESS` - `Kkm::Constants::FiscalProperty::PAYMENTS_ADDRESS`

- Убрана поддержка настроек передаваемых в формате XML, теперь поддерживается только Hash
- Убрана поддержка Docker контейнера

## 0.1.21

- Добавлена команда перезагрузки ККТ

## 0.1.20

- Убраны не используемые переменные в нативном коде
- Добавлены константы для тегов 1227, 1228, 1230, 1231
- Изменена версия руби для Dockerfile

## 0.1.19

- Изменены константы `Kkm::Constants::ChequeType`. Теперь они соответсвуют таблице 25 из ФЗ 54. Предыдущие значения для драйвера перенесены в класс `Kkm::Constants::ChequeTypeNumber`
- Убраны не используемые методы в `Kkm::Constants::ChequeTypeNumber`

## 0.1.18

- Добавлена возможность получения данных о фискальном документе по номеру
- Добавлена возможность добавлять дополнительные теги в позицию чека
- Исправлена ошибка отображения номера кода в ошибке `DeviceDriverError`
- Исправлена ошибка связанная с определением типа значения по умолчанию для тега
- Исправлен Dockerfile в связи с изменениями в зеркалах jessie/wheezy

## 0.1.17

- Исправлена ошибка связанная с установлением подключения, иногда метод `kkm_status` выдавал ошибку от предыдущего подключения. Теперь после установления соединения и перед закрытием соединения вызывается метод `reset_mode`

## 0.1.16

- Добавлены константы для тегов 1031, 1105

## 0.1.15

- Добавлены константы для тега 1199

## 0.1.14

- Добавлен метод для получения подрежима ККТ
- Поправлена ошибка с получением текущего режима и подрежима методом `current_mode`

## 0.1.13

- Добавлена возможность регистрировать составные теги
- Добавлены константы для тегов 1084, 1085, 1086 из ФЗ-54

## 0.1.12

- Добавлена возможность отправлять команды протокола Атол 3.1 в ККТ
- Добавлены константы для некоторых тегов из ФЗ-54
- Добавлены методы для получения статуса ККТ, статуса ФН и статуса обмена с ОФД
- Добавлен метод для получения текущего режима
- Добавлен метод для получения версии прошивки ФН
- Изменено поведение при установлении соеденения с ККТ. Вместо проверки соединения через `get_status`, проверка производится через `kkm_status`

## 0.1.11

- Доработан класс DeviceDriverError. Появилась возможность получать информацию о коде ошибке, названии устройства
- Добавлены метод для получения/установления настройки подключения по названию настройки
- Исправлена ошибка связанная не возможностью собрать docker контейнер

## 0.1.10

- Добавлены константы для фискального тега 1057

## 0.1.9

- Добавлены константы для фискальных тегов 1005, 1016, 1026, 1075, 1192
- Поправлены константы Признак предмета расчета для ФФД 1.05

## 0.1.8

- Добавлен метод для получение ИНН ФН
- Добавлен метод для получения номера последней смены в ФН
- Добавлен метод для получения номера чека в последней смене ФН
- Добавлен метод для получения сквозного номера чека
- Переменован метод `current_session` в `current_close_session`

## 0.1.7

- Добавлен метод для получения состояния текущей смены(открыта ли она и время открытия/закрытия)
- Добавлен метод для получения текущего времени в ККТ

## 0.1.6

- Добавлены методы для выплат/внесения денег в кассу
- Добавлена поддержка НДС 20%
- Добавлены методы для получения информации о регистрационном и заводском номерах ККТ
- Изменено определение налога `Kkm::DeviceInterface.setup_goods_info`. Теперь надо передавать константу налога, вместо символа/строки налога.
