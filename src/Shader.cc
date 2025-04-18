#include"Shader.hpp"

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader) {
  ID = compile_shader(vertex_shader, fragment_shader);
}

void Shader::use() {
  glUseProgram(ID);
}

void Shader::destroy() {
  glDeleteProgram(ID);
}

std::string read_file_to_string(const std::string& shader_file) {
  std::ifstream istrm {shader_file, std::ios::in};

  std::string source_string {};
  if(istrm.is_open()) {
    std::stringstream s;

    s << istrm.rdbuf();

    source_string = s.str();
  } else {
    std::cerr << "failed to open shader file: " << shader_file << std::endl;
    return std::string {""};
  }

  return source_string;
}

void check_stage_status(unsigned int id, GLenum pname) {
  int result {};

  if(pname == GL_LINK_STATUS){
    glGetProgramiv(id, pname, &result);
    if(result == GL_FALSE) {
      int maxLength {};
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
      char infoLog[512];
      glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);
      if(pname == GL_LINK_STATUS){
        std::cout << "Error linking" << std::endl;
      }

      std::printf("%s", infoLog);
    }
  } else {
    glGetShaderiv(id, pname, &result);

    if(result == GL_FALSE) {
      int maxLength {};
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
      char infoLog[512];
      glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);
      if(pname == GL_COMPILE_STATUS){
        std::cout << "Error compiling" << std::endl;
      }

      std::printf("%s", infoLog);
    }
  }
}

unsigned int compile_shader(const std::string& vertex_shader,
                            const std::string& fragment_shader) {

  std::string vertex_string = read_file_to_string(vertex_shader);
  std::string fragment_string = read_file_to_string(fragment_shader);

  unsigned int vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);

  const char* vertex_src = vertex_string.c_str();
  const char* fragment_src = fragment_string.c_str();

  glShaderSource(vertex_shader_ID, 1, &vertex_src, NULL);
  glShaderSource(fragment_shader_ID, 1, &fragment_src, NULL);

  glCompileShader(vertex_shader_ID);
  check_stage_status(vertex_shader_ID, GL_COMPILE_STATUS);
  glCompileShader(fragment_shader_ID);
  check_stage_status(fragment_shader_ID, GL_COMPILE_STATUS);

  unsigned int program = glCreateProgram();
  glAttachShader(program, vertex_shader_ID);
  glAttachShader(program, fragment_shader_ID);

  glLinkProgram(program);
  check_stage_status(program, GL_LINK_STATUS);

  glDetachShader(program, vertex_shader_ID);
  glDetachShader(program, fragment_shader_ID);

  glDeleteShader(vertex_shader_ID);
  glDeleteShader(fragment_shader_ID);

  return program;
}
