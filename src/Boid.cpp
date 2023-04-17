#include "Boid.hpp"

void Boid::draw(p6::Context& ctx, glm::mat4& viewMatrix)
{
    // std::vector<glimac::ShapeVertex> vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);
    // BoidRenderer                     boidRenderer(vertices);
    // boidRenderer.renderBoid(m_position, viewMatrix, ctx);
}

void Boid::updatePosition(p6::Context& ctx)
{
    m_position += ctx.delta_time() * m_speed;

    // std::cout << m_speed.x << " " << m_speed.y << " " << m_speed.z << std::endl;
    //   std::cout << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;
}

void Boid::setMaxSpeed(const float& speed)
{
    this->maxSpeed = speed;
}

void Boid::applyForce(const glm::vec3 force)
{
    this->m_speed += force;
}

void Boid::limitSpeed()
{
    if (glm::length(m_speed) > maxSpeed)
    {
        m_speed = glm::normalize(m_speed) * maxSpeed;
    }
}

bool operator==(const Boid& a, const Boid& b)
{
    return (a.getPosition() == b.getPosition()) && (a.getSpeed() == b.getSpeed());
}

bool operator!=(const Boid& a, const Boid& b)
{
    return !(a == b);
}