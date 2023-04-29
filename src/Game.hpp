#pragma once
#include <imgui.h>
#include <iostream>
#include <vector>
#include "Boid.hpp"
#include "Boids.hpp"
#include "Environment.hpp"
#include "Light.hpp"
#include "Player.hpp"
#include "glimac/Freefly.hpp"
#include "p6/p6.h"

class Game {
private:
    // Boids
    int   m_nbBoids = 50;
    Boids m_boids;

    // Player
    Player m_player;

    // Camera
    glimac::FreeflyCamera m_cam;
    glm::mat4             viewMatrix{0.f};
    float                 movementStrength = 100.f;
    float                 rotationStrength = 1000.f;

    // ImGui
    float protectedRadius    = 0.1f;
    float separationStrength = 0.1f;
    float alignmentStrength  = 0.1f;
    float cohesionStrength   = 0.1f;
    float maxSpeed           = 10.f;
    bool  lodLow             = false;
    bool  lodMid             = false;
    bool  lodHigh            = false;

    // OBJLoad
    std::vector<glimac::ShapeVertex>* verticesPtr; // pointeur pour pouvoir rediriger sur le bon lod
    std::vector<glimac::ShapeVertex>  verticesLow    = LoadOBJ("./assets/models/Night_Fury.obj");
    std::vector<glimac::ShapeVertex>  verticesMedium = LoadOBJ("./assets/models/LowDrake.obj");
    std::vector<glimac::ShapeVertex>  verticesHigh   = LoadOBJ("./assets/models/Drake_Obj.obj");

    // Environnement
    Environment m_environment;
    float       m_limit = 70.f;

    // Shader
    p6::Shader  m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/LightAndText.fs.glsl");
    Light       lightGame{m_shader};
    LightParams lightP;

    Light       lightPlayer{m_shader};
    LightParams lightP2;

    // Init methods
    void InitBoids(p6::Context& ctx, BoidsParameters& boidParam);
    void InitPlayer();
    void InitCamera();
    void InitImGui(BoidsParameters& boidParam);
    void InitEnvironment();
    void InitLight();
    void UpdateLightPlayer();
    void mouseHandler(p6::Context& ctx);
    void keyboardHandler(p6::Context& ctx);

    bool playerIsOutBorders();

public:
    Game(p6::Context& ctx, BoidsParameters& boidParam);
    void Render(p6::Context& ctx, BoidsParameters& boidParam);
};