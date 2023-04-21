#pragma once
#include <vcruntime.h>
#include <cstdlib>
#include <iterator>
#include <vector>
#include "Boid.hpp"
#include "glimac/plan_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "RendererBoids.hpp"
#include "p6/p6.h"

class Boids {
private:
    /*Attributes*/
    std::vector<Boid> m_boids;
    int               m_numBoids;

    float separationStrength;
    float alignmentStrength;
    float cohesionStrength;

    // float separationStrength = 0.2f;
    // float alignmentStrength  = 0.1f;
    // float cohesionStrength   = 0.1f;

    float turnfactor = 0.5f;
    float limit      = 40.f;

public:
    Boids() = default;

    Boids(const std::vector<Boid>& boids, const int& numBoids)
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

    // draw the boid
    void drawBoids(p6::Context& ctx, glm::mat4& viewMatrix);
    void drawBorders(p6::Context& ctx, glm::mat4& viewMatrix);

    // Help the boids to avoid edges
    void avoidEdges(Boid& boid, const float& limit, const float& turnfactor);

    // check distance between this boid and the boid in argument
    static bool isTooClose(const Boid& boid1, const Boid& boid2, const float& radius);
    // fill a vector of the neighbor
    std::vector<Boid> fillNeighbors(const Boid& boid, p6::Context& ctx);

    // use to draw a red circle when boids are too close
    void displayCollision(const std::vector<Boids>& neighbors, p6::Context& ctx) const;

    /*3 rules*/
    glm::vec3 separation(const Boid& boid) const;
    glm::vec3 alignment(const Boid& boid) const;
    glm::vec3 cohesion(const Boid& boid) const;

    // apply the 3 rules(separation, alignment, cohesion)
    void applySteeringForces(Boid& boid);
    // void updatePosition(p6::Context& ctx);

    void updateBoids(p6::Context& ctx)
    {
        for (auto& boid : m_boids)
        {
            std::vector<Boid> neighbors = fillNeighbors(boid, ctx);
            boid.updatePosition(ctx);
            applySteeringForces(boid);
            avoidEdges(boid, limit, turnfactor);
            neighbors.clear();
        }
    };
};
