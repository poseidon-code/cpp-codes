#! /usr/bin/env sh

# compiling library as Position Independent Code
g++ -c -fPIC -I. "./hello.cpp" -o "./hello.o"

# shared
g++ -shared -o "./libhello.so" "./hello.o"

# static
ar rs "./libhello.a" "./hello.o"
