#! /usr/bin/env sh

# run shared
export LD_LIBRARY_PATH="./hello":$LD_LIBRARY_PATH
./run_shared

# run static
./run_static
