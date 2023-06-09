#pragma once

#include "Boids.hpp"
#include "Environment.hpp"
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
    float protectedRadius    = 50.f;
    float visualRange        = 80.f;
    float separationStrength = 2.f;
    float alignmentStrength  = 1.f;
    float cohesionStrength   = 1.f;
    float maxSpeed           = 30.f;
    bool  lodLow             = true;
    bool  lodMid             = false;
    bool  lodHigh            = false;

    // OBJLoad
    std::vector<glimac::ShapeVertex>* verticesPtr; // pointer to redirect the good load
    std::vector<glimac::ShapeVertex>  verticesLow    = LoadOBJ("./assets/models/EnderDragon.obj");
    std::vector<glimac::ShapeVertex>  verticesMedium = LoadOBJ("./assets/models/Night_Fury.obj");
    std::vector<glimac::ShapeVertex>  verticesHigh   = LoadOBJ("./assets/models/Drake_Obj.obj");

    // Environnement
    Environment m_environment;
    float       m_limit = 570.f;

    // Shader
    p6::Shader m_shader       = p6::load_shader("shaders/3D.vs.glsl", "shaders/LightAndText.fs.glsl");
    p6::Shader m_shadowShader = p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl");

    // Light
    Light       lightGame{m_shader};
    LightParams lightP;
    Light       lightPlayer{m_shader};
    LightParams lightP2;

    // Shadow
    //  ShadowMapping m_shadowMap{m_shadowShader};
    //  ShadowMapping shadow{p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl")};

    // GLuint        m_DepthMap;
    // GLuint        m_DepthText;

    // Init methods
    void InitBoids();
    void InitPlayer();
    void InitCamera();
    void InitImGui(BoidsParameters& boidParam) const;
    void InitEnvironment();
    void InitLight();
    void UpdateLightPlayer();
    void MouseHandler(p6::Context& ctx);
    void KeyboardHandler(p6::Context& ctx);
    void ChangeLOD(BoidsParameters& boidParam);
    // void RenderShadow();

public:
    Game(p6::Context& ctx, BoidsParameters& boidParam);
    void RenderFinal(p6::Context& ctx, BoidsParameters& boidParam);
    void Render(p6::Context& ctx, BoidsParameters& boidParam);
};