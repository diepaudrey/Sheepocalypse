#include <iostream>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
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

    std::vector<Boid> boids;
    int               nb_boids = 50;

    Boids game(boids, nb_boids);
    game.fillBoids(ctx);
    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 10.f;

    // MVP
    glimac::FreeflyCamera camera           = glimac::FreeflyCamera();
    float                 movementStrength = 100.f;
    float                 rotationStrength = 1000.f;
    mouseHandler(ctx, camera, rotationStrength);

    glm::mat4 MVMatrix;
    glm::mat4 MVBMatrix;
    glm::mat4 NormalMatrix;

    /*Test class Mesh*/
    std::vector<glimac::ShapeVertex> verticesSphere = glimac::sphere_vertices(2.f, 32.f, 16);

    Mesh sphere(verticesSphere, verticesSphere.size());

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*Events*/
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        keyboardHandler(ctx, camera, movementStrength);

        MVMatrix     = camera.getViewMatrix();
        MVBMatrix    = camera.getViewMatrix();
        MVMatrix     = glm::rotate(MVMatrix, -ctx.time(), glm::vec3(0, 1, 0));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

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

        sphere.render(MVBMatrix, ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
