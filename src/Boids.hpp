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
    float maxSpeed;
    bool  lodLow  = false;
    bool  lodMid  = false;
    bool  lodHigh = false;

    void updateBoidsParam()
    {
        ImGui::Begin("Settings");
        ImGui::Text("To change settings please press SPACE");
        ImGui::SliderFloat("Protected Radius", &this->protectedRadius, 0.f, 3.f);
        ImGui::SliderFloat("Separation Strength", &this->separationStrength, 0.f, 5.f);
        ImGui::SliderFloat("Alignment Strength", &this->alignmentStrength, 0.f, 5.f);
        ImGui::SliderFloat("Cohesion Strength", &this->cohesionStrength, 0.f, 5.f);
        ImGui::SliderFloat("Max Speed", &this->maxSpeed, 0.1f, 30.f);
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
    float limit      = 50.f;

public:
    Boids() = default;

    Boids(const std::vector<Boid>& boids, const int& numBoids, BoidsParameters& boidParam)
        : m_boids(boids), m_numBoids(numBoids){};

    void fillBoids(p6::Context& ctx);

    /*Setters for ImGui*/
    void setProtectedRadius(const float& protecRad)
    {
        for (auto& boid : m_boids)
        {
            boid.setProtectedRadius(protecRad);
        }
    }

    void setSeparationStrength(const float& separation)
    {
        this->separationStrength = separation;
    }

    void setAlignmentStrength(const float& alignment)
    {
        this->alignmentStrength = alignment;
    }

    void setCohesionStrength(const float& cohesion)
    {
        this->cohesionStrength = cohesion;
    }

    void setBoidsMaxSpeed(const float& maxSpeed)
    {
        for (auto& boid : m_boids)
        {
            boid.setMaxSpeed(maxSpeed);
        }
    }

    size_t getBoidsSize()
    {
        return m_boids.size();
    }

    // draw the boid
    void drawBoids(p6::Context& ctx, glm::mat4& viewMatrix, std::vector<glimac::ShapeVertex> vertices, LightParams& lightP);
    void drawBorders(p6::Context& ctx, glm::mat4& viewMatrix);

    // Help the boids to avoid edges
    void avoidEdges(Boid& boid, const float& limit, const float& turnfactor, BoidsParameters& boidParam);

    // check distance between this boid and the boid in argument
    static bool isTooClose(const Boid& boid1, const Boid& boid2, const float& radius);
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const Boid& boid, p6::Context& ctx);

    // use to draw a red circle when boids are too close
    void displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const;

    /*3 rules*/
    glm::vec3 separation(const Boid& boid, BoidsParameters& boidParam) const;
    glm::vec3 alignment(const Boid& boid, BoidsParameters& boidParam) const;
    glm::vec3 cohesion(const Boid& boid, BoidsParameters& boidParam) const;

    // apply the 3 rules(separation, alignment, cohesion)
    void applySteeringForces(Boid& boid, BoidsParameters& boidParam);
    // void updatePosition(p6::Context& ctx);

    void updateBoids(p6::Context& ctx, BoidsParameters& boidParam);
};
