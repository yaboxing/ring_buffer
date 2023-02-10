#!/bin/bash
#set -x
set -u
set -e
#set -o pipefail

echo "prebuild start."

rm -rf gtest && mkdir gtest

cd third_party/googletest
mkdir build && cd build && cmake .. && make && cd ..

echo "install"

cp -raf build/lib/libgtest.a ../../gtest/
cp -raf googletest/include/gtest/* ../../gtest/

rm -rf build

echo "prebuild done."
