#include "Mesh.hpp"
#include "glad/glad.h"
#include <cstddef>

Vertex::Vertex(const glm::vec3 &pos, const glm::vec3 &color,
               const glm::vec2 &tex_coord)
: position(pos), color(color), tex_coords(tex_coord) {};

Mesh::Mesh(int numVAO, int numVBO) {
    glCreateVertexArrays(numVAO, &VAO);
    glCreateBuffers(numVBO, &VBO);
}

void Mesh::append_data() {

    GLsizeiptr ver_len = GLsizeiptr(sizeof(Vertex) * vertices.size());
    GLsizeiptr ind_len = GLsizeiptr(sizeof(indices[0]) * indices.size());
    glNamedBufferStorage(VBO, GLsizeiptr(ver_len + ind_len), nullptr,
                         GL_DYNAMIC_STORAGE_BIT);
    
    glNamedBufferSubData(VBO, 0, ver_len, vertices.data());
    glNamedBufferSubData(VBO, ver_len, ind_len, indices.data());

    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(VAO, VBO);

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
    unsigned long ind_offset = sizeof(Vertex) * vertices.size();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void *)ind_offset);
}
