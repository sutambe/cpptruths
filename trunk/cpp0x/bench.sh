#!/bin/bash

echo ${1-1000} functions
echo ${2-10} overloads per function
echo ${3-50} depth of hierarchy
echo ${4-5} arity

echo $'\nWith master dispatching function...'
python ./generate.py 1 $@ > test.cpp
time clang++ -fsyntax-only test.cpp
#rm test.cpp

echo $'\nWith one function per tag...'
python ./generate.py 0 $@ > test2.cpp
time clang++ -fsyntax-only test2.cpp
#rm test2.cpp
