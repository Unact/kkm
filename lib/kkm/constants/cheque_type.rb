module Kkm::Constants::ChequeType
  CHEQUE_CLOSED                   = 0  # Чек закрыт
  CHEQUE_SELL                     = 1  # Чек продажи / прихода
  CHEQUE_SELL_RETURN              = 2  # Чек возврат продажи / прихода
  CHEQUE_SELL_ANNULATE            = 3  # Чек аннулирования продажи
  CHEQUE_BUY                      = 4  # Чек покупки / расхода
  CHEQUE_BUY_RETURN               = 5  # Чек возврата покупки / расхода
  CHEQUE_BUY_ANNULATE             = 6  # Чек аннулирования покупки
  CHEQUE_SELL_CORRECTION          = 7  # Чек коррекции прихода
  CHEQUE_SELL_RETURN_CORRECTION   = 8  # Чек коррекции возврата прихода
  CHEQUE_BUY_CORRECTION           = 9  # Чек коррекции расхода
  CHEQUE_BUY_RETURN_CORRECTION    = 10 # Чек коррекции возврата расхода

  def self.find_type_by_value value
    constants.find{|constant| const_get(constant) == value}
  end

  def self.cheque_type_name value
    find_type_by_value(value).downcase.to_s
  end
end
