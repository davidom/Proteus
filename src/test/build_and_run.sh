if [ ! -d build ]
then
  mkdir build
  cd build
  cmake -DCMAKE_BUILD_TYPE=Release ../
  cd ..
fi

cd build
make
if [ $? -eq 0 ]
then
  ctest -V
fi
