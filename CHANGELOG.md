## 0.3.5

- Добавлена возможность внесения и выплаты наличных средств из ККТ

## 0.3.4

- Исправлена ошибка связанная с закрытием чека при возникновении ошибки при печати, если чек еще не был открыт

## 0.3.3

- Исправлена ошибка связанная с не корректным установлением кассира для чеков

## 0.3.2

- Исправлена ошибка связанная с оставлением чека в открытом состоянии вместо его отмены, при возникновении ошибки при печати
- Обновлена версия драйвера ДТО до 10.8.0.0

## 0.3.1

- Исправлена ошибка связанная с не работой флага electronically при открытии смены автоматически и при закрытии смены
- Исправлена ошибка при выполнении `KKM::Device.retrieve_data` с типом отчета `LibFptr::LIBFPTR_DT_DATE_TIME`
- Добавлена опция electronically в метод `KKM::Device.print_report`

## 0.3.0

**Breaking change** Корневой модуль переименован из `Kkm` в `KKM`  
**Breaking change** Сильно изменены сигнатуры и входящие параметры методов `KKM::Device`

- Убраны константы команд `KKM::Constants::Command`, так как они не поддерживаются платформой 5.0  
- Переименованы константы  

`KKM::Constants::FiscalProperty::CASHIER` - `KKM::Constants::FiscalProperty::OPERATOR`  
`KKM::Constants::FiscalProperty::CASHIER_INN` - `KKM::Constants::FiscalProperty::OPERATOR_VATIN`  
`KKM::Constants::FiscalProperty::BUYER_INN` - `KKM::Constants::FiscalProperty::BUYER_VATIN`  
`KKM::Constants::FiscalProperty::SUPPLIER_INN` - `KKM::Constants::FiscalProperty::SUPPLIER_VATIN`  
`KKM::Constants::FiscalProperty::ORGANIZATION_INN` - `KKM::Constants::FiscalProperty::ORGANIZATION_VATIN`  
`KKM::Constants::FiscalProperty::OFD_INN` - `KKM::Constants::FiscalProperty::OFD_VATIN`  
`KKM::Constants::FiscalProperty::TRANSFER_OPERATOR_INN` - `KKM::Constants::FiscalProperty::TRANSFER_OPERATOR_VATIN`  

- Переименованы все константы под `KKM::Constants::FiscalProperty` в `KKM::Constants::Tag`  
- Добавлены константы для тегов  

1173 - `KKM::Constants::Tag::CORRECTION_TYPE`  
1174 - `KKM::Constants::Tag::CORRECTION_DOCUMENT`  
1177 - `KKM::Constants::Tag::CORRECTION_DOCUMENT_NAME`  
1178 - `KKM::Constants::Tag::CORRECTION_DOCUMENT_DATETIME`  
1179 - `KKM::Constants::Tag::CORRECTION_DOCUMENT_NUMBER`  

- Убраны методы, не поддерживаемые платформой 5.0  

`KKM::Device.ofd_status_raw_data`  
`KKM::Device.mode_raw_data`  
`KKM::Device.fn_status_raw_data`  
`KKM::Device.status_raw_data`  

- Изменена инициализация класса `KKM::Device`, теперь на вход необходимо передавть экземпляр `KKM::Models::Settings`
- Добавлен метод `KKM::Device.execute_command` для выполнения команд
- Изменен метод `KKM::Device.print_receipt`, теперь на вход необходимо передавать экземпляр `KKM::Models::Receipt`
- Переименован метод `KKM::Device.setup_cashier` - `KKM::Device.setup_operator`, теперь на вход необходимо передавать экземпляр `KKM::Models::Operator`
- Изменен метод `print_report`, теперь на вход необходимо передавать экземпляр `KKM::Models::Report`
- Изменены методы `open_day` и `close_day`, теперь кассира надо передавать как экземпляр `KKM::Models::Operator`
- Изменен метод `print_text_line`, теперь текст для печати надо передавать как экземпляр `KKM::Models::TextLine`
- Объеденены методы в один метод `KKM::Device.retrieve_fn_data`. На вход необходимо передавать `KKM::Models::FNData`  

`KKM::Device.fn_last_receipt_data`  
`KKM::Device.fn_info_data`  
`KKM::Device.fn_registration_data`  
`KKM::Device.fn_shift_data`  

- Для `KKM::Device.retrieve_fn_data` с `KKM::Models::FNData` типа `LibFptr::LIBFPTR_FNDT_REG_INFO` изменены ответные теги  
`organization_inn` - `organization_vatin`  
`ofd_inn` - `ofd_vatin`  

- Объеденены методы в один метод `KKM::Device.retrieve_data`. На вход необходимо передавать `KKM::Models::Data`  

`KKM::Device.status_data`  
`KKM::Device.datetime`  
`KKM::Device.payment_register_data`  
`KKM::Device.shift_state_data`  
`KKM::Device.receipt_data`  

## 0.2.7

- Добавлена возможность регистрации итога чека и налога на чек

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
