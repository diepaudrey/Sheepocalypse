#pragma once
#include <imgui.h>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Environment.hpp"
#include "glimac/Freefly.hpp"
#include "p6/p6.h"

struct sImGui {
    float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float maxSpeed;
    bool  lod1;
    // float sizeWorld          = 100.f;
    sImGui();
    sImGui(float protecRad, float separaStrength, float alignStrength, float coheStrength, float mSpeed, bool lod)
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

class Game {
private:
    // Boids
    int   m_nbBoids = 50;
    Boids m_boids;

    // Camera
    glimac::FreeflyCamera m_cam;
    glm::mat4             viewMatrix;
    float                 movementStrength = 100.f;
    float                 rotationStrength = 1000.f;

    // ImGui
    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 10.f;
    bool  lodDragon          = false;
    // sImGui IHM;

    // OBJLoad
    std::vector<glimac::ShapeVertex>* verticesPtr; // pointeur pour pouvoir rediriger sur le bon lod
    std::vector<glimac::ShapeVertex>  verticesLow    = LoadOBJ("./assets/models/Wolf_One_obj.obj");
    std::vector<glimac::ShapeVertex>  verticesMedium = LoadOBJ("./assets/models/SmallArch_Obj.obj");
    std::vector<glimac::ShapeVertex>  verticesHigh   = LoadOBJ("./assets/models/Drake_Obj.obj");

    // Environnement
    Environment m_environment;

    // Init methods
    void InitBoids(p6::Context& ctx);
    void InitCamera();
    // void InitImGui();
    void InitEnvironment();

public:
    Game(p6::Context& ctx);

    void mouseHandler(p6::Context& ctx);
    void keyboardHandler(p6::Context& ctx);

    void Render(p6::Context& ctx);
};