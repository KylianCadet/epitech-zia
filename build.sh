#!/bin/sh

git submodule init
git submodule update

mkdir build && cd build && conan install .. --build=missing && cmake .. -G "Unix Makefiles" && cmake --build .