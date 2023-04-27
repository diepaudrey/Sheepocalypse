#include "Boid.hpp"

void Boid::updatePosition(p6::Context& ctx)
{
    m_position += ctx.delta_time() * m_speed;
}

void Boid::setMaxSpeed(const float& speed)
{
    this->maxSpeed = speed;
}

void Boid::applyForce(const glm::vec3 force)
{
    this->m_speed += force;
}

void Boid::limitSpeed(const float& maxSpeed)
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
