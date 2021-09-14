example for gtest with c/c++
===

# build
```
git clone https://github.com/zhexieren/ring_buffer.git
cd ring_buffer
make
```
# gtest dir
```
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake ..
make
cd ..
cp build/lib/libgtest.a ring_buffer/gtest/
cp googletest/include/gtest ring_buffer/gtest -raf
```

