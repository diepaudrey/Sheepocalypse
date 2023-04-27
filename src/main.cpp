#include <imgui.h>
#include <stddef.h>
#include <iostream>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Environment.hpp"
#include "Game.hpp"
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

struct BoidsParameters {
    float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float maxSpeed;
    bool  lod1;
    // float sizeWorld          = 100.f;

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
    auto ctx = p6::Context{{1280, 720, "Dragons"}};
    ctx.maximize_window();

    Game TestGame(ctx);

    /* Loop until the user closes the window */
    ctx.update = [&]() {
        /*Events*/
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        TestGame.Render(ctx);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}
