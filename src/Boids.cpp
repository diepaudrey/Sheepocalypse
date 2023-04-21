#include "Boids.hpp"

void Boids::drawBoids(p6::Context& ctx, glm::mat4& viewMatrix)
{
    std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(5.f, 3.f, 32, 16);

    /* Pour faire un plan, vertices = sphere(surface plan, 2 , 2)*/
    // std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(20.f, 4.f, 2.f);
    RendererBoids boidRenderer(vertices);
    boidRenderer.renderBoids(m_boids, viewMatrix, ctx);
    boidRenderer.deleteBuffers();
}

// void Boids::drawBorders(p6::Context& ctx, glm::mat4& viewMatrix)
// {
//     Glfloat vertices[] = {

//     };
//     Renderer borders(vertices);
//     borders.Render();
//     borders.deleteBuffers();
// }

void Boids::fillBoids(p6::Context& ctx)
{
    const float& min = 20.f;
    const float& max = 20.f;

    for (int i = 0; i < m_numBoids; i++)
    {
        glm::vec3 pos(p6::random::number(-min, max), p6::random::number(-min, max), p6::random::number(-min, max));
        glm::vec3 speed(p6::random::number(-min, max), p6::random::number(-min, max), p6::random::number(-min, max));
        // std::cout << speed.x << " " << speed.y << " " << speed.z << std::endl;
        Boid boid(pos, speed);

        m_boids.push_back(boid);
    }
}

void Boids::avoidEdges(Boid& boid, const float& limit, const float& turnfactor)
{
    if (boid.getPosition().x + boid.getProtectedRadius() > limit)
    {
        boid.addSpeedX(-turnfactor);
    }
    if (boid.getPosition().x - boid.getProtectedRadius() < -limit)
    {
        boid.addSpeedX(turnfactor);
    }

    if (boid.getPosition().y + boid.getProtectedRadius() > limit)
    {
        boid.addSpeedY(-turnfactor);
    }
    if (boid.getPosition().y - boid.getProtectedRadius() < -limit)
    {
        boid.addSpeedY(turnfactor);
    }

    if (boid.getPosition().z + boid.getProtectedRadius() > limit)
    {
        boid.addSpeedZ(-turnfactor);
    }
    if (boid.getPosition().z - boid.getProtectedRadius() < -limit)
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
    const float separationRange   = 4.5f;
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
        steeringForce = steeringForce * separationStrength;
    }
    // std::cout << "Steering force : " << steeringForce.x << " " << steeringForce.y << " " << steeringForce.z << std::endl;

    return steeringForce;
}

glm::vec3 Boids::alignment(const Boid& boid) const
{
    glm::vec3 averageDirection(0.f, 0.f, 0.f);
    float     alignmentRange = 5.f;
    // float     meanAlignment     = 0.0f;
    int numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, alignmentRange))
        {
            float distance = glm::distance(boid.getPosition(), otherBoid.getPosition());

            averageDirection += otherBoid.getSpeed() / distance;

            // std::cout << "Other Boid detected " << std::endl;
            // meanAlignment += 1.0f / distance;
            numberOfNeighbors++;
        }
    }
    // std::cout << "Number of neighbors : " << numberOfNeighbors << std::endl;

    if (numberOfNeighbors != 0)
    {
        averageDirection *= alignmentStrength;
        averageDirection /= numberOfNeighbors;
        averageDirection = normalize(averageDirection);
    }
    // std::cout << "Average direction : " << averageDirection.x << " " << averageDirection.y << " " << averageDirection.z << std::endl;
    return averageDirection;
}

glm::vec3 Boids::cohesion(const Boid& boid) const
{
    glm::vec3 averageLocation(0.f, 0.f, 0.f);
    float     cohesionRange     = 2.f;
    int       numberOfNeighbors = 0;

    for (const auto& otherBoid : m_boids)
    {
        if (isTooClose(boid, otherBoid, cohesionRange))
        {
            // float distance = glm::distance(boid.getPosition(), otherBoid.getPosition());
            //  std::cout << "distance: " << distance << std::endl;
            averageLocation += otherBoid.getPosition();
            numberOfNeighbors++;
        }
    }

    if (numberOfNeighbors != 0)
    {
        averageLocation /= numberOfNeighbors;
        averageLocation = normalize(averageLocation - boid.getPosition()) * cohesionStrength;
    }
    // std::cout << "Average location : " << averageLocation.x << " " << averageLocation.y << " " << averageLocation.z << std::endl;

    return averageLocation;
}

void Boids::applySteeringForces(Boid& boid)
{
    boid.applyForce(alignment(boid));
    boid.applyForce(cohesion(boid));
    boid.applyForce(separation(boid));
    boid.limitSpeed();
}
