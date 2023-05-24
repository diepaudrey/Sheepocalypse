#pragma once
#include <imgui.h>
#include <vcruntime.h>
#include <cstdlib>
#include <iterator>
#include <vector>
#include "Boid.hpp"
#include "OBJLoader.hpp"
#include "RendererBoids.hpp"
#include "glimac/plan_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

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

    void updateBoidsParam()
    {
        ImGui::Begin("Settings");
        ImGui::Text("To change settings please press SPACE");
        ImGui::SliderFloat("Protected Radius", &this->protectedRadius, 0.f, 50.f);
        ImGui::SliderFloat("Visual Range", &this->visualRange, 0.f, 50.f);
        ImGui::SliderFloat("Separation Strength", &this->separationStrength, 0.f, 5.f);
        ImGui::SliderFloat("Alignment Strength", &this->alignmentStrength, 0.f, 5.f);
        ImGui::SliderFloat("Cohesion Strength", &this->cohesionStrength, 0.f, 5.f);
        ImGui::SliderFloat("Max Speed", &this->maxSpeed, 0.f, 30.f);
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
    int               m_numBoids;

    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;

    float turnfactor = 0.5f;
    float limit      = 500.f;

    /*Methods*/

    /*3 rules*/
    static glm::vec3 separation(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);
    static glm::vec3 alignment(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);
    static glm::vec3 cohesion(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);

    // apply the 3 rules(separation, alignment, cohesion)
    static void applySteeringForces(Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors);

    // check distance between this boid and the boid in argument
    static bool isTooClose(const Boid& boid1, const Boid& boid2, const float& radius);
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const Boid& boid, BoidsParameters& boidParam);

public:
    Boids() = default;

    Boids(const std::vector<Boid>& boids, const int& numBoids)
        : m_boids(boids), m_numBoids(numBoids){};

    void fillBoids();
    void drawBoids(p6::Context& ctx, glm::mat4& viewMatrix, std::vector<glimac::ShapeVertex> vertices, LightParams& lightP);
    void updateBoids(p6::Context& ctx, BoidsParameters& boidParam);
};
