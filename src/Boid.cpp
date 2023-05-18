#include "Boid.hpp"

void Boid::updatePosition(p6::Context& ctx)
{
    m_position += ctx.delta_time() * m_speed;
}

void Boid::avoidEdges(Boid& boid, const float& limit, const float& turnfactor, const float& protectedRadius)
{
    if (boid.getPosition().x + protectedRadius > limit)
    {
        boid.m_speed.x -= turnfactor;
    }
    if (boid.getPosition().x - protectedRadius < -limit)
    {
        boid.m_speed.x += turnfactor;
    }

    if (boid.getPosition().y + protectedRadius > limit)
    {
        boid.m_speed.y -= turnfactor;
    }
    if (boid.getPosition().y - protectedRadius < -limit)
    {
        boid.m_speed.y += turnfactor;
    }

    if (boid.getPosition().z + protectedRadius > limit)
    {
        boid.m_speed.z -= turnfactor;
    }
    if (boid.getPosition().z - protectedRadius < -limit)
    {
        boid.m_speed.z += turnfactor;
    }
}

void Boid::applyForce(const glm::vec3& force)
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
