#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

struct Display {
  int width;
  int height;
  GLFWwindow* window;

  Display(int width, int height);
  void destroy();
  void clear();
  void swap();
};
