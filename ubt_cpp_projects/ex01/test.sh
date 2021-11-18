#!/bin/bash

g++ -c -o my_test main.cpp *.o
./my_test $*
rm my_test
