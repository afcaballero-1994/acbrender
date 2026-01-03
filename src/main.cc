#include "Display.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <assimp/mesh.h>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtx/string_cast.hpp>

void processMesh(Mesh *obj, aiNode *node, const aiScene *scene) {
    std::cout << "NumMeshes: " << scene->mNumMeshes << std::endl;
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *m = scene->mMeshes[node->mMeshes[i]];
        std::cout << "Vertices: " << m->mNumFaces << std::endl;

        unsigned long int offset = obj->vertices.size();
        for (size_t j = 0; j < m->mNumVertices; j++) {
            glm::vec3 pos, norm;
            glm::vec2 texcord;
            pos.x = m->mVertices[j].x;
            pos.y = m->mVertices[j].y;
            pos.z = m->mVertices[j].z;
            norm.x = m->mNormals[j].x;
            norm.y = m->mNormals[j].y;
            norm.z = m->mNormals[j].z;
            if (m->mTextureCoords[0]) {
                texcord.x = m->mTextureCoords[0][j].x;
                texcord.y = m->mTextureCoords[0][j].y;
            } else {
                texcord = glm::vec2(0.0f, 0.0f);
            }

            obj->vertices.emplace_back(Vertex{pos, norm, texcord});
        }
        for (size_t k = 0; k < m->mNumFaces; k++) {
            aiFace face = m->mFaces[k];
            for (size_t l = 0; l < face.mNumIndices; l++) {
                obj->indices.emplace_back(face.mIndices[l] + offset);
            }
        }
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        processMesh(obj, node->mChildren[i], scene);
    }

    std::cout << obj->indices.size() << std::endl;
}

int main() {
    constexpr unsigned int scale = 100;
    constexpr unsigned int width_window = 16 * scale;
    constexpr unsigned int height_window = 9 * scale;
    Display display(width_window, height_window);

    Shader shader =
        Shader("./resources/triangle.vert", "./resources/triangle.frag");

    Assimp::Importer importer;
    const char *pfile = "./resources/backpack/backpack.obj";

    const aiScene *scene =
        importer.ReadFile(pfile, aiProcess_Triangulate | aiProcess_FlipUVs |
                                     aiProcess_GenSmoothNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        std::cout << "Error::Assimp::" << importer.GetErrorString()
                  << std::endl;
        return 1;
    }

    Mesh object = Mesh();

    aiNode *n = scene->mRootNode;
    processMesh(&object, n, scene);

    object.append_data();

    Texture tex1 = Texture("./resources/backpack/diffuse.jpg");
    tex1.bind(0);
    glm::mat4 projection{glm::mat4(1.0f)};
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)width_window / (float)height_window,
                                  0.1f, 100.0f);

    while (!glfwWindowShouldClose(display.window)) {

        glfwPollEvents();

        if (glfwGetKey(display.window, GLFW_KEY_R)) {
            std::cout << "Reloading shader " << shader.ID << std::endl;
            reload_shader(shader.ID, "./resources/triangle.vert",
                          "./resources/triangle.frag");
            std::cout << "New shader " << shader.ID << std::endl;
        }

        if (glfwGetKey(display.window, GLFW_KEY_ESCAPE)) {
            break;
        }
        shader.use();

        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view{glm::mat4(1.0f)};
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

        glUniform2f(3, width_window, height_window);
        glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(6, 1, GL_FALSE, glm::value_ptr(model));

        display.clear();
        object.draw();
        display.swap();
    }

    shader.destroy();
    object.destroy();

    display.destroy();

    return 0;
}

void reload_shader(unsigned int &program, const std::string &vertex_shader,
                   const std::string &fragment_shader) {

    unsigned int reloaded_program =
        compile_shader(vertex_shader, fragment_shader);

    if (reloaded_program) {
        glDeleteProgram(program);
        program = reloaded_program;
    }
}
