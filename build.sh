#!/bin/bash

set -ouex pipefail

INCLUDE_DIR="include"
IMGUI_DIR="include/imgui"
SRC_DIR="src"
LIBS="-lglfw -ldl -lassimp -lGL -lX11"
OUTPUT="crender"
CXX="clang++"
CXXFLAGS="-I$INCLUDE_DIR -I$IMGUI_DIR -I$IMGUI_DIR/backends -std=c++23 -Wextra -Wall -Wshadow -Wpointer-arith -Wcast-align -Wconversion -Wunreachable-code -pedantic -g"

SRC_FILES=$(find $SRC_DIR -name "*.cc" -o -name "*.c")
SRC_FILES+=" $IMGUI_DIR/imgui.cpp $IMGUI_DIR/imgui_demo.cpp $IMGUI_DIR/imgui_draw.cpp $IMGUI_DIR/imgui_tables.cpp $IMGUI_DIR/imgui_widgets.cpp "
SRC_FILES+=" $IMGUI_DIR/backends/imgui_impl_glfw.cpp $IMGUI_DIR/backends/imgui_impl_opengl3.cpp "

#$CXX $SRC_FILES $CXXFLAGS -o main $LIBS

$CXX $SRC_FILES $CXXFLAGS -o main $LIBS

./main
