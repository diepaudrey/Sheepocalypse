#include <stddef.h>
#include <iostream>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "OBJLoader.hpp"
#include "RendererBoids.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "glimac/Freefly.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

void mouseHandler(p6::Context& ctx, glimac::FreeflyCamera& camera, const float& rotationStrength)
{
    /*La caméra sans souris*/

    // glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ctx.mouse_moved = [&](p6::MouseMove data) {
    //     (camera).rotateLeft(data.delta.x * rotationStrength);
    //     (camera).rotateUp(-data.delta.y * rotationStrength);
    // };

    /*La caméra avec cliquer + glisser*/
    ctx.mouse_dragged = [&](p6::MouseDrag data) {
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
    std::cout << GL_TEXTURE0 + 1 << std::endl;

    std::vector<Boid> boids;
    int               nb_boids = 50;

    Boids game(boids, nb_boids);
    game.fillBoids(ctx);
    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 10.f;

    /*VBO*/ /*VAO*/
    // cone
    std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 32, 16);
    Vbo                              vbo(vertices.data(), vertices.size());
    glEnable(GL_DEPTH_TEST);

    Vao vao;
    vao.AddBuffer(vbo);
    vbo.Bind();
    vao.UnBind();

    // MVP
    glimac::FreeflyCamera camera = glimac::FreeflyCamera();
    // glm::mat4             viewMatrix       = camera.getViewMatrix();
    float movementStrength = 100.f;
    float rotationStrength = 1000.f;
    mouseHandler(ctx, camera, rotationStrength);

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 1280 / static_cast<float>(720), 0.1f, 100.f);
    glm::mat4 MVMatrix;
    glm::mat4 MVBMatrix;
    glm::mat4 NormalMatrix;
    glm::vec3 light = glm::vec3(1.f, 1.f, 1.f);

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
        RotDir.emplace_back(glm::linearRand(0, 1), glm::linearRand(0, 1), glm::linearRand(0, 1));
        _uKa.emplace_back(glm::linearRand(0.f, 0.05f), glm::linearRand(0.f, 0.05f), glm::linearRand(0.f, 0.05f));
        _uKd.emplace_back(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f));
        _uKs.emplace_back(glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f), glm::linearRand(0.f, 1.0f));
        _uShininess.push_back(glm::linearRand(0.f, 1.0f));
    }

    /*Test class Mesh*/
    // std::vector<glimac::ShapeVertex> verticesSphere = glimac::sphere_vertices(2.f, 32.f, 16);

    // Mesh sphere(verticesSphere, verticesSphere.size());

    /*Test OBJ loader*/
    std::vector<glimac::ShapeVertex> verticesWolf;
    verticesWolf = LoadOBJ("./assets/models/SmallArch_Obj.obj");
    std::vector<Texture> textures;
    Texture              m_texture{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Diffuse.png")};
    Texture              m_textureH{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Height.png"), 1};
    Texture              m_textureN{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Normal.png"), 2};
    Texture              m_textureS{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Specular.png"), 1};
    textures.push_back(m_texture);
    textures.push_back(m_textureH);
    textures.push_back(m_textureN);
    textures.push_back(m_textureS);
    std::cout << "vector textures size : " << textures.size() << std::endl;

    Mesh loup(verticesWolf, verticesWolf.size(), textures, textures.size());

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*Events*/
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        keyboardHandler(ctx, camera, movementStrength);

        vbo.Bind();
        vao.Bind();
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

        vbo.UnBind();

        vao.UnBind();
        glBindVertexArray(0);

        /*Dear ImGui*/
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Protected Radius", &protectedRadius, 0.f, 2.f);
        ImGui::SliderFloat("Separation Strength", &separationStrength, 0.f, 10.f);
        ImGui::SliderFloat("Alignment Strength", &alignmentStrength, 0.f, 2.f);
        ImGui::SliderFloat("Cohesion Strength", &cohesionStrength, 0.f, 2.f);
        ImGui::SliderFloat("Max Speed", &maxSpeed, 0.f, 5.f);
        ImGui::End();

        /*GAME*/

        game.setProtectedRadius(protectedRadius);
        game.setAlignmentStrength(alignmentStrength);
        game.setCohesionStrength(cohesionStrength);
        game.setSeparationStrength(separationStrength);
        game.setBoidsMaxSpeed(maxSpeed);

        game.updateBoids(ctx);
        game.drawBoids(ctx, MVBMatrix);

        loup.Render(MVBMatrix, ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    // delete vbo
    vbo.DeleteVbo();
    vao.DeleteVao();
}
