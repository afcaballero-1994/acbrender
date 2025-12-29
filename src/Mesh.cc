#include "Mesh.hpp"
#include <cstddef>

Vertex::Vertex(const glm::vec3 &pos, const glm::vec3 &color,
               const glm::vec2 &tex_coord)
: position(pos), color(color), tex_coords(tex_coord) {};

Mesh::Mesh(int numVAO, int numVBO) {
    vertices.reserve(300);
    glCreateVertexArrays(numVAO, &VAO);
    glCreateBuffers(numVBO, &VBO);
}

void Mesh::append_data() {

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));

    unsigned long int ver_len = sizeof(Vertex) * vertices.size();
    unsigned long int ind_len = sizeof(glm::uvec3) * indices.size();

    glNamedBufferStorage(VBO, ver_len + ind_len, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(VBO, 0, ver_len, vertices.data());
    glNamedBufferSubData(VBO, ver_len, ind_len, indices.data());

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glEnableVertexArrayAttrib(VAO, 2);

    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE,
                              offsetof(Vertex, position));
    glVertexArrayAttribFormat(VAO, 1, 3, GL_FLOAT, GL_FALSE,
                              offsetof(Vertex, color));
    glVertexArrayAttribFormat(VAO, 2, 2, GL_FLOAT, GL_FALSE,
                              offsetof(Vertex, tex_coords));

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
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, indices.data());
}
