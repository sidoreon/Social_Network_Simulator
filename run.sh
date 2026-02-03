#!/bin/bash

echo "Compiling socialnet.cpp"
g++ -std=c++17 -O2 -Wall -Wextra socialnet.cpp -o socialnet

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    echo "Running socialnet"
    ./socialnet
else
    echo "Compilation failed."
    exit 1
fi
