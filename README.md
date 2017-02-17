# kkm

Установка Qt
sudo apt-get install qt5-default

Создание so
cd ./cpp_so/build
qmake ../FptrExample/FptrExample.pro
cd ../..
cp ./cpp_so/build/libFptrTest.so.1.0.0 ./bins/linux-x64/libKkm.so


