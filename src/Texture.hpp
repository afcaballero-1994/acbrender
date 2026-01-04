#pragma once

#include <glad/glad.h>
#include <iostream>
#include <string>

struct Texture {
    unsigned int ID;
    std::string type;
    Texture(char const *filename, const std::string &type = "diffuse");
    void bind(unsigned int loc);
    void destroy();
};
