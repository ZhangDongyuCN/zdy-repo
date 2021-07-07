#!/bin/bash
if test ! -e build
then
	mkdir build
fi
cd build
cmake ..
make -j4
