#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                      GLsizei length, GLchar const* message, void const* user_name);
void check_state_status(unsigned int id, GLenum pname);

unsigned int compile_shader(const std::string& vertex_shader,
                            const std::string& fragment_shader);
void reload_shader(unsigned int &program, const std::string& vertex_shader,
                   const std::string& fragment_shader);

constexpr unsigned int scale         = 100;
constexpr unsigned int width_window  = 16 * scale;
constexpr unsigned int height_window = 9  * scale;

struct Vertex {
  glm::vec3 pos;
  glm::vec3 col;

  Vertex(const glm::vec3 pos, const  glm::vec3 col) : pos(pos), col(col) {};
};

void window_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
  
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(
                                        width_window,
                                        height_window,
                                        "acbrender", NULL, NULL);
  if(window == NULL) {
    std::cerr << "failed to create window" << std::endl;
    return -1;
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "failed to initialize GLAD" << std::endl;

    return -1;
  }

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(message_callback, nullptr);

  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);

  glViewport(0, 0, width_window, height_window);
  Vertex v1 = Vertex(glm::vec3{-0.5f, -0.5f, 0.0f}, glm::vec3{1.0f, 0.0f, 0.0f});
  Vertex v2 = Vertex(glm::vec3{0.5f, -0.5f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
  Vertex v3 = Vertex(glm::vec3{0.0f, 0.5f, 0.0f}, glm::vec3{0.0f, 0.0f, 1.0f});

  std::vector<Vertex> vertices {v1, v2, v3};
  unsigned int VBO, VAO;

  glCreateBuffers(1, &VBO);
  glNamedBufferStorage(VBO, sizeof(Vertex) * vertices.size(), vertices.data(),
                       GL_DYNAMIC_STORAGE_BIT);
  glCreateVertexArrays(1, &VAO);
  glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));

  glEnableVertexArrayAttrib(VAO, 0);
  glEnableVertexArrayAttrib(VAO, 1);
  glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
  glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, col));

  glVertexArrayAttribBinding(VAO, 0, 0);
  glVertexArrayAttribBinding(VAO, 1, 0);

  glBindVertexArray(VAO);

  unsigned int shader = compile_shader("./resources/triangle.vert",
                                       "./resources/triangle.frag");

  while(!glfwWindowShouldClose(window)) {

    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_R)) {
      std::cout << "Reloading shader " << shader << std::endl;
      reload_shader(shader, "./resources/triangle.vert",
                    "./resources/triangle.frag");
      std::cout << "New shader " << shader << std::endl;
    }

    glUseProgram(shader);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();

  return 0;
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
      char *infoLog = nullptr;
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
      char *infoLog = nullptr;
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

  return program;
}

void reload_shader(unsigned int &program, const std::string& vertex_shader,
                   const std::string& fragment_shader) {

  unsigned int reloaded_program = compile_shader(vertex_shader, fragment_shader);

  if(reloaded_program) {
    glDeleteProgram(program);
    program = reloaded_program;
  }
}

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, 
                      GLsizei length, GLchar const* message, void const* user_param) {
  auto const src_str = [source]() {
    switch (source)
      {
      case GL_DEBUG_SOURCE_API: return "API";
      case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
      case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
      case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
      case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
      case GL_DEBUG_SOURCE_OTHER: return "OTHER";
      }
  }();

  auto const type_str = [type]() {
    switch (type)
      {
      case GL_DEBUG_TYPE_ERROR: return "ERROR";
      case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
      case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
      case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
      case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
      case GL_DEBUG_TYPE_MARKER: return "MARKER";
      case GL_DEBUG_TYPE_OTHER: return "OTHER";
      }
  }();

  auto const severity_str = [severity]() {
    switch (severity) {
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
    case GL_DEBUG_SEVERITY_LOW: return "LOW";
    case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
    case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
    }
  }();
  std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

void window_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
