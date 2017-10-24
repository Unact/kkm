module Kkm::Constants::PositionType
  # Таблица из Протокола ККТ 3.1
  GOODS             = 1  # Товар
  LICENSE_GOODS     = 2  # Подакциозный товар
  WORK              = 3  # Работа
  SERVICE           = 4  # Услуга
  GAME_BET          = 5  # Ставка азартной игры
  GAME_WIN          = 6  # Выигрыш азартной игры
  LOTTERY_TICKET    = 7  # Лотерейный билет
  LOTTERY_WIN       = 8  # Выгрыш лотереи
  INTELLECT_RESULT  = 9  # Предоставление РИД (Результата интеллектуальной деятельности)
  PAYMENT           = 10 # Платеж
  COMPOSITE         = 11 # Составной предмет расчета
  OTHER             = 12 # Иной предмет расчета
end
