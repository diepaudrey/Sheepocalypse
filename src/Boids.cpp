#include "Boids.hpp"
#include <vcruntime.h>
#include <iterator>
#include "glm/fwd.hpp"

void Boids::drawBoids(p6::Context& ctx, glm::mat4& viewMatrix)
{
    std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 32, 16);
    Renderer                         boidRenderer(vertices);
    boidRenderer.renderBoids(m_boids, viewMatrix, ctx);
    boidRenderer.deleteBuffers();
}

void Boids::fillBoids(p6::Context& ctx)
{
    for (int i = 0; i < m_numBoids; i++)
    {
        glm::vec3 pos(p6::random::number(-5.f, 5.f), p6::random::number(-5.f, 5.f), p6::random::number(-5.f, 5.f));
        glm::vec3 speed(p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f), p6::random::number(-2.f, 2.f));
        // std::cout << speed.x << " " << speed.y << " " << speed.z << std::endl;
        Boid boid(pos, speed);

        m_boids.push_back(boid);
    }
}

void Boids::avoidEdges(Boid& boid, const p6::Context& ctx, const float& turnfactor)
{
    if (boid.getPosition().x + boid.getProtectedRadius() > 5)
    {
        boid.addSpeedX(-turnfactor);
    }
    if (boid.getPosition().x - boid.getProtectedRadius() < -5)
    {
        boid.addSpeedX(turnfactor);
    }

    if (boid.getPosition().y + boid.getProtectedRadius() > 5)
    {
        boid.addSpeedY(-turnfactor);
    }
    if (boid.getPosition().y - boid.getProtectedRadius() < -5)
    {
        boid.addSpeedY(turnfactor);
    }

    if (boid.getPosition().z + boid.getProtectedRadius() > 5)
    {
        boid.addSpeedZ(-turnfactor);
    }
    if (boid.getPosition().z - boid.getProtectedRadius() < -5)
    {
        boid.addSpeedZ(turnfactor);
    }
}

bool Boids::isTooClose(const Boid& boid1, const Boid& boid2, const float& radius)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition()) < radius && boid1 != boid2;
}

std::vector<Boid> Boids::fillNeighbors(const Boid& boid, p6::Context& ctx)
{
    std::vector<Boid> neighbors;
    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, boid.getProtectedRadius()))
        {
            neighbors.push_back(otherBoid);
            ctx.fill = {1.f, 0.f, 0.f, 0.3f};
        }
    }
    return neighbors;
}

/* 3 Rules of the game*/

glm::vec3 Boids::separation(const Boid& boid) const
{
    glm::vec3   steeringForce(0.f, 0.f, 0.f);
    const float separationRange   = 0.3f;
    int         numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, separationRange))

        {
            float distance = glm::distance(boid.getPosition(), otherBoid.getPosition());
            steeringForce += (boid.getPosition() - otherBoid.getPosition()) / distance;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        steeringForce /= numberOfNeighbors;
        steeringForce = normalize(steeringForce) * separationStrength;
    }

    return steeringForce;
}

glm::vec3 Boids::alignment(const Boid& boid) const
{
    glm::vec3 averageDirection(0.f, 0.f, 0.f);
    float     alignmentRange    = 0.2f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, alignmentRange))
        {
            averageDirection += otherBoid.getSpeed();
            // std::cout << "Other Boid speed  " << otherBoid.m_speed.x << " " << otherBoid.m_speed.y << std::endl;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageDirection *= alignmentStrength;
        averageDirection /= numberOfNeighbors;
    }
    return averageDirection;
}

glm::vec3 Boids::cohesion(const Boid& boid) const
{
    glm::vec3 averageLocation(0.f, 0.f, 0.f);
    float     cohesionRange     = 0.5f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, cohesionRange))
        {
            averageLocation += otherBoid.getPosition();
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = (averageLocation - boid.getPosition()) * cohesionStrength;
    }

    return averageLocation;
}

void Boids::applySteeringForces(Boid& boid)
{
    boid.applyForce(alignment(boid));
    boid.applyForce(cohesion(boid));
    boid.applyForce(separation(boid));
    boid.limitSpeed();
}

// void Boids::deleteAllBuffers()
// {
//     for (const auto& boid : m_boids)
//     {
//         boid.delete()
//     }
// }
