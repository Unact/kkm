class Kkm::Device
  def initialize settings
    @device_driver = Kkm::DeviceInterface.new settings
  end

  # Если возникает ошибка, ККМ обязательно надо выключить
  def work(&block)
      @device_driver.turn_on
      yield @device_driver
      @device_driver.turn_off
    rescue => error
      @device_driver.turn_off
      raise error.message
  end
end
