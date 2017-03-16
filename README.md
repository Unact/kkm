# Kkm

Работает только под Linux.
Для работы с гемом создан Dockerfile.
Сборка: `docker build --no-cache -t kkm .`
Запуск: `docker run -i -t --rm -v "$(pwd)":/app -w /app kkm`


Для работы с устройством Ккм необходимо создать экземпляр `Kkm::DeviceInterface`
