#include <iostream>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "glimac/Freefly.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

void mouseHandler(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& rotationStrength)
{
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ctx.mouse_moved = [&](p6::MouseMove data) {
        (camera).rotateLeft(data.delta.x * rotationStrength);
        (camera).rotateUp(-data.delta.y * rotationStrength);
    };
}

void keyboardHandler(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& mvtStrength)
{
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        camera.moveFront(ctx.delta_time() * mvtStrength);
    };
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        camera.moveFront(-ctx.delta_time() * mvtStrength);
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        camera.moveLeft(ctx.delta_time() * mvtStrength);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        camera.moveLeft(-ctx.delta_time() * mvtStrength);
    }
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "Light"}};
    ctx.maximize_window();
    const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/pointLight.fs.glsl");
    Light            light_scene(shader);

    std::vector<Boid> boids;
    int               nb_boids = 25;

    Boids game(boids, nb_boids);
    game.fillBoids(ctx);
    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 1.2f;

    /*VBO*/
    // cone
    std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);
    VertexBuffer                     vbo(vertices.data(), vertices.size());
    glEnable(GL_DEPTH_TEST);

    // GLuint                           vbo;
    // GLuint                           vao;
    // std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);
    // BoidRenderer                     boid(vertices, vbo, vao);

    /*VAO*/
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)nullptr);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // MVP

    glimac::FreeflyCamera camera           = glimac::FreeflyCamera();
    float                 movementStrength = 5.f;
    float                 rotationStrength = 900.f;
    mouseHandler(ctx, camera, rotationStrength);

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 1280 / static_cast<float>(720), 0.1f, 100.f);
    glm::mat4 MVMatrix;
    glm::mat4 MVBMatrix;
    glm::mat4 NormalMatrix;
    glm::vec3 light = glm::vec3(1.f, 0.f, 0.f);

    // For the light
    glm::mat4              MVMatrix_light;
    glm::mat4              NormalMatrix_light;
    std::vector<glm::vec3> RotAxes;
    std::vector<glm::vec3> RotDir;
    std::vector<glm::vec3> _uKa;
    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;
    for (int i = 0; i < 32; i++)
    {
        RotAxes.push_back(glm::ballRand(2.f));
        RotDir.push_back(glm::vec3(glm::linearRand(0, 1), glm::linearRand(0, 1), glm::linearRand(0, 1)));
        _uKa.push_back(glm::vec3(glm::linearRand(0.f, 0.05f), glm::linearRand(0.f, 0.05f), glm::linearRand(0.f, 0.05f)));
        _uKd.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        _uKs.push_back(glm::vec3(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f)));
        _uShininess.push_back(glm::linearRand(0.f, 1.0f));
    }

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*Events*/
        keyboardHandler(ctx, camera, movementStrength);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vbo.Bind();
        glBindVertexArray(vao);
        shader.use();

        glm::vec3 uLightPos   = glm::vec3(glm::rotate(glm::mat4(1.f), ctx.delta_time(), glm::vec3(0, 1, 0)) * glm::vec4(light, 1));
        glm::vec3 uMVLightPos = glm::vec3(camera.getViewMatrix() * glm::vec4(uLightPos, 1));
        MVMatrix              = camera.getViewMatrix();
        MVBMatrix             = camera.getViewMatrix();
        MVMatrix              = glm::rotate(MVMatrix, -ctx.time(), glm::vec3(0, 1, 0));
        NormalMatrix          = glm::transpose(glm::inverse(MVMatrix));

        glUniformMatrix4fv(light_scene.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(light_scene.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(light_scene.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glUniform3fv(light_scene.m_uKa, 1, glm::value_ptr(glm::vec3(0.0215, 0.1745, 0.0215)));
        glUniform3fv(light_scene.m_uKd, 1, glm::value_ptr(glm::vec3(0.07568, 0.61424, 0.07568)));
        glUniform3fv(light_scene.m_uKs, 1, glm::value_ptr(glm::vec3(0.633, 0.727811, 0.633)));
        glUniform1f(light_scene.m_uShininess, 0.6);

        glUniform3fv(light_scene.m_uLightPos_vs, 1, glm::value_ptr(glm::vec3(glm::rotate(camera.getViewMatrix(), ctx.time(), glm::vec3(0, 1, 0)) * glm::vec4(1, 1, 0, 1))));
        glUniform3fv(light_scene.m_uLightIntensity, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        for (int i = 0; i < 32; i++)
        {
            MVMatrix_light     = camera.getViewMatrix();
            MVMatrix_light     = glm::rotate(MVMatrix_light, ctx.time(), glm::vec3(RotDir[i][0], RotDir[i][1], RotDir[i][2]));
            MVMatrix_light     = glm::translate(MVMatrix_light, RotAxes[i]);
            MVMatrix_light     = glm::scale(MVMatrix_light, glm::vec3(0.2, 0.2, 0.2));
            NormalMatrix_light = glm::transpose(glm::inverse(MVMatrix_light));

            glUniformMatrix4fv(light_scene.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix_light));
            glUniformMatrix4fv(light_scene.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix_light));
            glUniformMatrix4fv(light_scene.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix_light));

            glUniform3fv(light_scene.m_uKa, 1, glm::value_ptr(_uKa[i]));
            glUniform3fv(light_scene.m_uKd, 1, glm::value_ptr(_uKd[i]));
            glUniform3fv(light_scene.m_uKs, 1, glm::value_ptr(_uKs[i]));
            glUniform1f(light_scene.m_uShininess, _uShininess[i]);

            glUniform3fv(light_scene.m_uLightPos_vs, 1, glm::value_ptr(uMVLightPos));
            glUniform3fv(light_scene.m_uLightIntensity, 1, glm::value_ptr(glm::vec3(1, 1, 1)));

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
        game.drawBoids(ctx, MVBMatrix);
        game.updateBoids(ctx, MVBMatrix);
        vbo.UnBind();
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteVertexArrays(0, &vao);
}
