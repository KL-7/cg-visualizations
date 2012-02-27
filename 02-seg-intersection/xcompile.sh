#!/bin/bash
make clean && i686-pc-mingw32-qmake && make
qmake && make clean
