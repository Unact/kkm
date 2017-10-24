module Kkm::Constants::PositionPaymentType
  # Таблица из Протокола ККТ 3.1
  PREPAYMENT_100    = 1  # Предоплата 100%
  PREPAYMENT        = 2  # Предоплата
  PREPAID           = 3  # Аванс
  FULL_PAYMENT      = 4  # Полный расчет
  PARTIAL_PAYMENT   = 5  # Частичный расчет
  CREDIT            = 6  # Передача в кредит
  CREDIT_PAYMENT    = 7  # Оплата кредита
end
