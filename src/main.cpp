#include <imgui.h>
#include <stddef.h>
#include <iostream>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Environment.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "OBJLoader.hpp"
#include "RendererBoids.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "glimac/Freefly.hpp"
#include "glimac/plan_vertices.hpp"
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

struct setImGui {
    float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float maxSpeed;
    bool  lod1;
    // float sizeWorld          = 100.f;

    setImGui(float protecRad, float separaStrength, float alignStrength, float coheStrength, float mSpeed, bool lod)
    {
        protectedRadius    = protecRad;
        separationStrength = separaStrength;
        alignmentStrength  = alignStrength;
        cohesionStrength   = coheStrength;
        maxSpeed           = mSpeed;
        lod1               = lod;
    };

    void updateImGui()
    {
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Protected Radius", &this->protectedRadius, 0.f, 3.f);
        ImGui::SliderFloat("Separation Strength", &this->separationStrength, 0.f, 5.f);
        ImGui::SliderFloat("Alignment Strength", &this->alignmentStrength, 0.f, 5.f);
        ImGui::SliderFloat("Cohesion Strength", &this->cohesionStrength, 0.f, 5.f);
        ImGui::SliderFloat("Max Speed", &this->maxSpeed, 0.1f, 30.f);
        ImGui::Checkbox("LOD 1", &lod1);
        ImGui::End();
    }
};

int main()
{
    auto ctx = p6::Context{{1280, 720, "Light"}};
    ctx.maximize_window();
    const p6::Shader shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/pointLight.fs.glsl");
    // Light            light_scene(shader);
    // std::cout << GL_TEXTURE0 + 1 << std::endl;

    std::vector<Boid> boids;
    int               nb_boids = 50;

    Boids game(boids, nb_boids);
    game.fillBoids(ctx);

    float    protectedRadius    = 0.1f;
    float    separationStrength = 0.1f;
    float    alignmentStrength  = 0.1f;
    float    cohesionStrength   = 0.1f;
    float    maxSpeed           = 10.f;
    bool     lodDragon          = false;
    setImGui IHM(protectedRadius, separationStrength, alignmentStrength, cohesionStrength, maxSpeed, lodDragon);

    std::vector<glimac::ShapeVertex>* vertices_ptr; // pointeur pour pouvoir rediriger sur le bon lod
    std::vector<glimac::ShapeVertex>  vertices      = LoadOBJ("./assets/models/Drake_Obj.obj");
    std::vector<glimac::ShapeVertex>  vertices_wolf = LoadOBJ("./assets/models/Wolf_One_obj.obj");

    // Environment world;
    // world.InitBorders();
    // world.InitArche();

    glEnable(GL_DEPTH_TEST);

    // MVP
    glimac::FreeflyCamera camera = glimac::FreeflyCamera();
    glm::mat4             viewMatrix;
    float                 movementStrength = 100.f;
    float                 rotationStrength = 1000.f;
    mouseHandler(ctx, camera, rotationStrength);

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*Events*/
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        keyboardHandler(ctx, camera, movementStrength);
        viewMatrix = camera.getViewMatrix();

        IHM.updateImGui();

        if (IHM.lod1 == true)
        {
            vertices_ptr = &vertices_wolf;
        }
        else
        {
            vertices_ptr = &vertices;
        }

        /*GAME*/
        game.setProtectedRadius(IHM.protectedRadius);
        game.setAlignmentStrength(IHM.alignmentStrength);
        game.setCohesionStrength(IHM.cohesionStrength);
        game.setSeparationStrength(IHM.separationStrength);
        game.setBoidsMaxSpeed(IHM.maxSpeed);

        game.updateBoids(ctx);
        game.drawBoids(ctx, viewMatrix, *vertices_ptr);

        // world.RenderBorders(viewMatrix, ctx);
        // world.RenderArche(viewMatrix, ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
