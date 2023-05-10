#pragma once
#include <cstdlib>
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_position;
    glm::vec3 m_speed;
    float     maxSpeed;
    float     protectedRadius;

public:
    Boid() = default;
    Boid(const glm::vec3& pos, const glm::vec3 vel)
        : m_position(pos), m_speed(vel){};

    glm::vec3 getPosition() const
    {
        return m_position;
    }
    glm::vec3 getSpeed() const
    {
        return m_speed;
    }

    float getProtectedRadius() const
    {
        return protectedRadius;
    }

    float getMaxSpeed() const
    {
        return maxSpeed;
    }

    void addSpeedX(const float speed)
    {
        m_speed.x += speed;
    }

    void addSpeedY(const float speed)
    {
        m_speed.y += speed;
    }

    void addSpeedZ(const float speed)
    {
        m_speed.z += speed;
    }

    void setProtectedRadius(const float protRad)
    {
        protectedRadius = protRad;
    }

    void updatePosition(p6::Context& ctx);
    void setMaxSpeed(const float& speed);
    void applyForce(const glm::vec3 force);

    // limit the speed
    void limitSpeed(const float& maxSpeed);

    friend bool operator==(const Boid& a, const Boid& b);
    friend bool operator!=(const Boid& a, const Boid& b);
};