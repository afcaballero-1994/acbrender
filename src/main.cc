#include "Display.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

int main() {
    constexpr unsigned int scale = 100;
    constexpr unsigned int width_window = 16 * scale;
    constexpr unsigned int height_window = 9 * scale;
    Display display(width_window, height_window);

    Shader shader =
        Shader("./resources/triangle.vert", "./resources/triangle.frag");
    std::vector<glm::vec3> positions{glm::vec3{0.5f, 0.5f, 0.0f},
                                     glm::vec3{1.0f - 0.5f, -1.0f + 0.5f, 0.0f},
                                     glm::vec3{-1.0f + 0.5f, -1.0f + 0.5f, 0.0f},
                                     glm::vec3{-1.0f + 0.5f, 1.0f - 0.5f, 0.0f}};
    std::vector<glm::vec3> colors{
        glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f},
        glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{1.0f, 1.0f, 1.0f}};
    std::vector<glm::vec2> tex_coords{
        glm::vec2{1.0f, 1.0f}, glm::vec2{1.0f, 0.0f}, glm::vec2{0.0f, 0.0f},
        glm::vec2{0.0f, 1.0f}};
    std::vector<glm::uvec3> indices{glm::uvec3{0, 1, 3}, glm::uvec3{1, 2, 3}};
    std::vector<Vertex> vtices;

    Mesh mesh = Mesh();
    for (unsigned long i = 0; i != colors.size(); i++) {
        vtices.emplace_back(Vertex{positions[i], colors[i], tex_coords[i]});
    }
    mesh.vertices = vtices;
    mesh.indices = indices;
    mesh.append_data();

    Texture tex1 = Texture("./resources/a.png");
    tex1.bind(0);
    glm::mat4 projection{glm::mat4(1.0f)};
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)width_window / (float)height_window,
                                  0.1f, 100.0f);
    std::cout << glm::to_string(projection) << std::endl;

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
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glUniform2f(3, width_window, height_window);
        glUniformMatrix4fv(5, 1, GL_FALSE, glm::value_ptr(view));

        glm::mat4 model = glm::mat4(1.0f);
        model =
            glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(6, 1, GL_FALSE, glm::value_ptr(model));

        display.clear();
        ;
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        mesh.draw();
        display.swap();
    }

    shader.destroy();
    mesh.destroy();

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
