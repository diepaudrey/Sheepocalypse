#include "Boids.hpp"

void Boids::DrawBoids(p6::Context& ctx, glm::mat4& viewMatrix, std::vector<glimac::ShapeVertex> vertices, LightParams& lightP)
{
    RendererBoids boidRenderer(vertices, lightP);
    boidRenderer.RenderBoids(m_boids, viewMatrix, ctx, lightP);
    boidRenderer.DeleteBuffers();
}

void Boids::FillBoids()
{
    for (int i = 0; i < m_numBoids; i++)
    {
        glm::vec3 pos(p6::random::number(-limit, limit), p6::random::number(-limit, limit), p6::random::number(-limit, limit));
        glm::vec3 speed(p6::random::number(-limit, limit), p6::random::number(-limit, limit), p6::random::number(-limit, limit));
        Boid      boid(pos, speed);

        m_boids.push_back(boid);
    }
}

bool Boids::IsTooClose(const Boid& boid1, const Boid& boid2, const float& radius)
{
    return glm::distance(boid1.GetPosition(), boid2.GetPosition()) < radius && boid1 != boid2;
}

std::vector<Boid> Boids::FillNeighbors(const Boid& boid, BoidsParameters& boidParam)
{
    std::vector<Boid> neighbors;
    for (const auto& otherBoid : m_boids)
    {
        if (IsTooClose(boid, otherBoid, boidParam.visualRange))
        {
            neighbors.push_back(otherBoid);
        }
    }
    return neighbors;
}

/* 3 Rules of the game*/

glm::vec3 Boids::Separation(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec3 steeringForce(0.f, 0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (IsTooClose(boid, otherBoid, boidParam.protectedRadius))

        {
            float distance = glm::distance(boid.GetPosition(), otherBoid.GetPosition());
            steeringForce += (boid.GetPosition() - otherBoid.GetPosition()) / distance;
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

glm::vec3 Boids::Alignment(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec3 averageDirection(0.f, 0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (IsTooClose(boid, otherBoid, boidParam.visualRange))
        {
            float distance = glm::distance(boid.GetPosition(), otherBoid.GetPosition());

            averageDirection += otherBoid.GetSpeed() / distance;
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

glm::vec3 Boids::Cohesion(const Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    glm::vec3 averageLocation(0.f, 0.f, 0.f);
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : neighbors)
    {
        if (IsTooClose(boid, otherBoid, boidParam.visualRange))
        {
            averageLocation += otherBoid.GetPosition();
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = normalize(averageLocation - boid.GetPosition()) * boidParam.cohesionStrength;
    }

    return averageLocation;
}

void Boids::ApplySteeringForces(Boid& boid, BoidsParameters& boidParam, const std::vector<Boid>& neighbors)
{
    boid.ApplyForce(Alignment(boid, boidParam, neighbors));
    boid.ApplyForce(Cohesion(boid, boidParam, neighbors));
    boid.ApplyForce(Separation(boid, boidParam, neighbors));
    boid.LimitSpeed(boidParam.maxSpeed);
}

void Boids::UpdateBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    boidParam.UpdateBoidsParam();
    for (auto& boid : m_boids)
    {
        std::vector<Boid> neighbors = FillNeighbors(boid, boidParam);
        boid.UpdatePosition(ctx);
        ApplySteeringForces(boid, boidParam, neighbors);
        Boid::AvoidEdges(boid, limit, turnfactor, boidParam.protectedRadius);
        Boid::AvoidMapElements(boid, turnfactor);
        neighbors.clear();
    }
};
