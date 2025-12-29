#pragma once

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec2 tex_coords;
  Vertex(const glm::vec3 &pos, const glm::vec3 &color, const glm::vec2 &tex_coord);
};

struct Mesh {
  unsigned int VBO, VAO;
  std::vector<Vertex> vertices;
  std::vector<glm::uvec3> indices;

  Mesh(int numVAO = 1, int numVBO = 1);
  void append_data();
  void draw();
  void destroy();
};
