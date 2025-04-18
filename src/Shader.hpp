#pragma once

#include<glad/glad.h>
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>

struct Shader {
  unsigned int ID;
  void use();
  void destroy();

  Shader(const std::string& vertex_shader,
         const std::string& fragment_shader);
  
};

unsigned int compile_shader(const std::string& vertex_shader,
                            const std::string& fragment_shader);

void check_state_status(unsigned int id, GLenum pname);

void reload_shader(unsigned int &program, const std::string& vertex_shader,
                   const std::string& fragment_shader);

std::string read_file_to_string(const std::string& shader_file);
