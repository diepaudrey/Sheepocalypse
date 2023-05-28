#pragma once
#include "RendererBoids.hpp"

struct BoidsParameters {
    float protectedRadius;
    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;
    float visualRange;
    float maxSpeed;
    bool  lodLow  = false;
    bool  lodMid  = false;
    bool  lodHigh = false;

    void UpdateBoidsParam()
    {
        ImGui::Begin("Settings");
        ImGui::Text("To change settings please press SPACE");
        ImGui::SliderFloat("Protected Radius", &this->protectedRadius, 0.f, 200.f);
        ImGui::SliderFloat("Visual Range", &this->visualRange, 0.f, 200.f);
        ImGui::SliderFloat("Separation Strength", &this->separationStrength, 0.f, 10.f);
        ImGui::SliderFloat("Alignment Strength", &this->alignmentStrength, 0.f, 10.f);
        ImGui::SliderFloat("Cohesion Strength", &this->cohesionStrength, 0.f, 10.f);
        ImGui::SliderFloat("Max Speed", &this->maxSpeed, 0.f, 200.f);
        ImGui::Checkbox("LoD Low", &lodLow);
        ImGui::Checkbox("LoD Medium", &lodMid);
        ImGui::Checkbox("LoD High", &lodHigh);
        ImGui::End();
    }
};

class Boids {
private:
    /*Attributes*/
    std::vector<Boid> m_boids;
    int               m_numBoids{};

    float turnfactor = 2.f;
    float limit      = 400.f;

    /*3 rules*/
    static glm::vec3 Separation(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);
    static glm::vec3 Alignment(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);
    static glm::vec3 Cohesion(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);

    // apply the 3 rules(separation, alignment, cohesion)
    static void ApplySteeringForces(Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);

    // check distance between this boid and the boid in argument
    static bool IsTooClose(const Boid& boid1, const Boid& boid2, const float& radius);

    // fill a vector of the neighbor
    std::vector<Boid> FillNeighbors(const Boid& boid, BoidsParameters& boidParam);

public:
    Boids() = default;

    Boids(const std::vector<Boid>& boids, const int& numBoids)
        : m_boids(boids), m_numBoids(numBoids){};

    void FillBoids();
    void DrawBoids(p6::Context& ctx, glm::mat4& viewMatrix, std::vector<glimac::ShapeVertex> vertices, LightParams& lightP);
    void UpdateBoids(p6::Context& ctx, BoidsParameters& boidParam);
};
