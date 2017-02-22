# kkm

Работает только под  linux

Установка Qt
```
sudo apt-get install qt5-default
```
Создание so:
```
cd ./cpp_so/build
rm *
qmake ../kkm_interface.pro
make
cd ../..
```