#!/bin/bash

echo "Compiling LA2.cpp"
g++ -std=c++17 -O2 -Wall -Wextra LA2.cpp -o socialnet

if [ $? -eq 0 ]; then
    echo "Compilation successful."
    echo "Running socialnet"
    ./socialnet
else
    echo "Compilation failed."
    exit 1
fi