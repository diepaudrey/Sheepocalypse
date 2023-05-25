#include "Boids.hpp"

void Boids::drawBoids(p6::Context& ctx, glm::mat4& viewMatrix, std::vector<glimac::ShapeVertex> vertices, LightParams& lightP)
{
    RendererBoids boidRenderer(vertices, lightP);
    boidRenderer.RenderBoids(m_boids, viewMatrix, ctx, lightP);
    boidRenderer.DeleteBuffers();
}

void Boids::fillBoids()
{
    const float& min = 400.f;
    const float& max = 400.f;

    for (int i = 0; i < m_numBoids; i++)
    {
        glm::vec3 pos(p6::random::number(-min, max), p6::random::number(-min, max), p6::random::number(-min, max));
        glm::vec3 speed(p6::random::number(-min, max), p6::random::number(-min, max), p6::random::number(-min, max));
        Boid      boid(pos, speed);

        m_boids.push_back(boid);
    }
}

bool Boids::isTooClose(const Boid& boid1, const Boid& boid2, const float& radius)
{
    return glm::distance(boid1.getPosition(), boid2.getPosition()) < radius && boid1 != boid2;
}

std::vector<Boid> Boids::fillNeighbors(const Boid& boid, BoidsParameters& boidParam)
{
    std::vector<Boid> neighbors;
    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, boidParam.visualRange))
        {
            neighbors.push_back(otherBoid);
        }
    }
    return neighbors;
}

/* 3 Rules of the game*/

glm::vec3 Boids::separation(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec3 steeringForce(0.f, 0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (isTooClose(boid, otherBoid, boidParam.protectedRadius))

        {
            float distance = glm::distance(boid.getPosition(), otherBoid.getPosition());
            steeringForce += (boid.getPosition() - otherBoid.getPosition()) / distance;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        steeringForce /= numberOfNeighbors;
        steeringForce = steeringForce * boidParam.separationStrength;
    }

    return steeringForce;
}

glm::vec3 Boids::alignment(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec3 averageDirection(0.f, 0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (isTooClose(boid, otherBoid, boidParam.visualRange))
        {
            float distance = glm::distance(boid.getPosition(), otherBoid.getPosition());

            averageDirection += otherBoid.getSpeed() / distance;
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageDirection /= numberOfNeighbors;
        averageDirection = normalize(averageDirection);
        averageDirection *= boidParam.alignmentStrength;
    }
    return averageDirection;
}

glm::vec3 Boids::cohesion(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec3 averageLocation(0.f, 0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (isTooClose(boid, otherBoid, boidParam.visualRange))
        {
            averageLocation += otherBoid.getPosition();
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = normalize(averageLocation - boid.getPosition()) * boidParam.cohesionStrength;
    }

    return averageLocation;
}

void Boids::applySteeringForces(Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    boid.applyForce(alignment(boid, boidParam, neighbors));
    boid.applyForce(cohesion(boid, boidParam, neighbors));
    boid.applyForce(separation(boid, boidParam, neighbors));
    boid.limitSpeed(boidParam.maxSpeed);
}

void Boids::updateBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    boidParam.updateBoidsParam();
    for (auto& boid : m_boids)
    {
        std::vector<Boid> neighbors = fillNeighbors(boid, boidParam);
        boid.updatePosition(ctx);
        applySteeringForces(boid, boidParam, neighbors);
        Boid::avoidEdges(boid, limit, turnfactor, boidParam.protectedRadius);
        Boid::avoidMapElements(boid, turnfactor);
        neighbors.clear();
    }
};
