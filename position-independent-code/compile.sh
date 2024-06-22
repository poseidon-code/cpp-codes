#! /usr/bin/env sh

# using shared
# (automatically links shared library by default, if no shared library is found, then links static library)
g++ -L"./hello" -I"./hello" -o "./run_shared" "./main.cpp" -lhello

# using static
# (linking static/shared library directly)
g++ -I"./hello" -o "./run_static" "./main.cpp" "./hello/libhello.a"
