#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>

struct Mesh {
  unsigned int VBO, VAO;
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> colors;
  std::vector<glm::vec3> tex_coords;
  std::vector<glm::uvec3> indices;

  Mesh(int numVAO = 1, int numVBO = 1);
  void append_data(const std::vector<glm::vec3>& pos,
                   const std::vector<glm::vec3>& col,
                   const std::vector<glm::vec3>& tex_coord,
                   const std::vector<glm::uvec3>& index);
  void draw();
  void destroy();
};

  
