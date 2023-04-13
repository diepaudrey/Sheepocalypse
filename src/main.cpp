

#include "Boid.hpp"
#include "BoidRenderer.hpp"
#include "Boids.hpp"
#include "cstddef"
#include "glimac/Freefly.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
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
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*Camera*/
    // glimac::TrackballCamera camera;
    glimac::FreeflyCamera camera;

    float movementStrength = 10.f;
    float rotationStrength = 900.f;

    mouseHandler(ctx, camera, rotationStrength);
    std::vector<Boid> boids;
    int               nb_boids = 25;

    Boids game(boids, nb_boids);
    game.fillBoids(ctx);
    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 1.2f;

    // GLuint                           vbo;
    // GLuint                           vao;
    // std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);
    // BoidRenderer                     boid(vertices, vbo, vao);

    // /*Loading Shader*/
    // const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

    // /*Location uniform variables*/
    // GLint uMVPMatrix = glGetUniformLocation(shader.id(), "uMVPMatrix");

    // GLint uMVMatrix = glGetUniformLocation(shader.id(), "uMVMatrix");

    // GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);

    mouseHandler(ctx, camera, rotationStrength);
    // Declare your infinite update loop.
    ctx.update = [&]() {
        glm::mat4 viewMatrix = camera.getViewMatrix();
        /*Dear ImGui*/
        // ImGui::Begin("Settings");
        // ImGui::SliderFloat("Protected Radius", &protectedRadius, 0.f, 2.f);
        // ImGui::SliderFloat("Separation Strength", &separationStrength, 0.f, 1.f);
        // ImGui::SliderFloat("Alignment Strength", &alignmentStrength, 0.f, 1.f);
        // ImGui::SliderFloat("Cohesion Strength", &cohesionStrength, 0.f, 1.f);
        // ImGui::SliderFloat("Max Speed", &maxSpeed, 0.f, 5.f);
        // ImGui::End();

        /*Events*/
        keyboardHandler(ctx, camera, movementStrength);

        // game.setProtectedRadius(protectedRadius);
        // game.setAlignmentStrength(alignmentStrength);
        // game.setCohesionStrength(cohesionStrength);
        // game.setSeparationStrength(separationStrength);
        // game.setBoidsMaxSpeed(maxSpeed);
        game.drawBoids(ctx, viewMatrix);
        game.updateBoids(ctx, viewMatrix);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}