module Kkm::Constants::FiscalPropertyType
  RAW                = 0 # Сырые данные
  UNSIGNED_SHORTINT  = 1 # Беззнаковое целое(0..255)
  UNSIGNED_INT       = 2 # Беззнаковое целое(0..65535)
  UNSIGNED_BIGINT    = 3 # Беззнаковое целое(0..4294967295)
  UNIXTIME           = 4
  STRING             = 5 # Строка

  def self.infer_type value
    case value
    when Integer
      case value
      when 0..255
        UNSIGNED_SHORTINT
      when 256..65535
        UNSIGNED_INT
      else
        UNSIGNED_BIGINT
      end
    when DateTime, Time
      UNIXTIME
    when String
      STRING
    else
      RAW
    end
  end
end
