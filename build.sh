#!/bin/bash

set -ouex pipefail

INCLUDE_DIR="include"
SRC_DIR="src"
LIBS="-lglfw -ldl"
OUTPUT="crender"
CXX="g++"
CXXFLAGS="-I$INCLUDE_DIR -std=c++23 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align -Wconversion -Wunreachable-code -pedantic -g"

SRC_FILES=$(find $SRC_DIR -name "*.cc" -o -name "*.c")

$CXX $SRC_FILES $CXXFLAGS -o main $LIBS

bear -- $CXX $SRC_FILES $CXXFLAGS -o main $LIBS

./main
