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
    // Light            light_scene(shader);
    // std::cout << GL_TEXTURE0 + 1 << std::endl;

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

    /*Test OBJ loader*/
    std::vector<glimac::ShapeVertex> verticesWolf;
    verticesWolf = LoadOBJ("./assets/models/SmallArch_Obj.obj");
    std::vector<Texture> textures;
    Texture              m_texture{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Diffuse.png"), 3};
    Texture              m_textureH{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Height.png"), 2};
    Texture              m_textureN{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Normal.png")};
    Texture              m_textureS{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Specular.png"), 1};

    // Texture m_texture{p6::load_image_buffer("assets/textures/Drake.jpg")};
    // Texture m_textureH{p6::load_image_buffer("assets/textures/lila.png"), 1};
    textures.push_back(m_textureH);
    textures.push_back(m_texture);

    // textures.push_back(m_textureN);
    textures.push_back(m_textureS);

    Mesh loup(verticesWolf, verticesWolf.size(), textures, textures.size());

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
        game.drawBoids(ctx, viewMatrix);
        loup.Render(viewMatrix, ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
