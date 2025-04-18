#include"Mesh.hpp"

Mesh::Mesh(int numVAO, int numVBO) {
  glCreateVertexArrays(numVAO, &VAO);
  glCreateBuffers(numVBO, &VBO);
}

void Mesh::append_data(const std::vector<glm::vec3>& pos,
                       const std::vector<glm::vec3>& col,
                       const std::vector<glm::vec3>& tex_coord,
                       const std::vector<glm::uvec3>& index) {

  positions.insert(positions.end(), pos.begin(), pos.end());
  colors.insert(colors.end(), col.begin(), col.end());
  tex_coords.insert(tex_coords.end(), tex_coord.begin(), tex_coord.end());
  indices.insert(indices.end(), index.begin(), index.end());
  
  glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(float) * 3);

  unsigned int pos_len = sizeof(float) * positions.size() * 3;
  unsigned int col_len = sizeof(float) * positions.size() * 3;
  unsigned int tex_len = sizeof(float) * positions.size() * 3;
  unsigned int ind_len = sizeof(float) * indices.size() * 3;
  
  glNamedBufferStorage(VBO, pos_len + col_len + tex_len  + ind_len, nullptr, GL_DYNAMIC_STORAGE_BIT);
  glNamedBufferSubData(VBO, 0      , pos_len, positions.data());
  glNamedBufferSubData(VBO, pos_len, col_len, colors.data());
  glNamedBufferSubData(VBO, pos_len + col_len, tex_len, tex_coords.data());
  glNamedBufferSubData(VBO, pos_len + col_len + tex_len, ind_len, indices.data());

  glEnableVertexArrayAttrib(VAO, 0);
  glEnableVertexArrayAttrib(VAO, 1);
  glEnableVertexArrayAttrib(VAO, 2);

  glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE, pos_len);
  glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE, pos_len + col_len);
  
  glVertexArrayAttribBinding(VAO, 0, 0);
  glVertexArrayAttribBinding(VAO, 1, 0);
  glVertexArrayAttribBinding(VAO, 2, 0);

  glBindVertexArray(VAO);
}

void Mesh::destroy() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void Mesh::draw() {
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices.data());
}
