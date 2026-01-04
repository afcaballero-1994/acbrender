#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <assimp/mesh.h>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "Display.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtx/string_cast.hpp>

void processMesh(Mesh *obj, aiNode *node, const aiScene *scene) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh *m = scene->mMeshes[node->mMeshes[i]];

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
    std::cout << "Model size: mib"
              << (sizeof(Vertex) * object.vertices.size() >> 20) << std::endl;
    object.append_data();

    Texture tex1 = Texture("./resources/backpack/diffuse.jpg");
    tex1.bind(0);
    glm::mat4 projection{glm::mat4(1.0f)};
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)width_window / (float)height_window,
                                  0.1f, 100.0f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    std::cout << "pas" << std::endl;
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    std::cout << "bio" << std::endl;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsLight();
    float mainScale =
        ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplGlfw_InitForOpenGL(display.window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    bool showImGuiPanel = true;
    ImVec4 cpos = ImVec4(0.0, 0.0f, -6.0f, 0.0f);
    float angle = 0.0f;
    ImVec4 crotation = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(display.window)) {

        glfwPollEvents();
        if (glfwGetWindowAttrib(display.window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        if (glfwGetKey(display.window, GLFW_KEY_R)) {
            std::cout << "Reloading shader " << shader.ID << std::endl;
            reload_shader(shader.ID, "./resources/triangle.vert",
                          "./resources/triangle.frag");
            std::cout << "New shader " << shader.ID << std::endl;
        }

        if (glfwGetKey(display.window, GLFW_KEY_ESCAPE)) {
            break;
        }
        if (glfwGetKey(display.window, GLFW_KEY_D)) {
            showImGuiPanel = !showImGuiPanel;
        }
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        shader.use();
        if (showImGuiPanel) {
            ImGui::Begin("Settings");
            if (ImGui::CollapsingHeader("Camera")) {
                ImGui::SliderFloat3("Camera position", (float *)&cpos, -15.0f,
                                    15.0f);
                ImGui::SliderFloat("Rotation angle", &angle, 0.0f, 180.0f);
                ImGui::SliderFloat3("Rotation axis", (float *)&crotation, 0.0f,
                                    1.0f);
            }
            ImGui::End();
        }

        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 view{glm::mat4(1.0f)};
        view = glm::translate(view, glm::vec3(cpos.x, cpos.y, cpos.z));
        view = glm::rotate(view, angle,
                           glm::vec3(crotation.x, crotation.y, crotation.z));

        glUniform2f(3, width_window, height_window);
        glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(6, 1, GL_FALSE, glm::value_ptr(model));
        ImGui::Render();
        display.clear();
        object.draw();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
