#pragma once
#include <cstdlib>
#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_position;
    glm::vec3 m_speed;

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

    void        updatePosition(p6::Context& ctx);
    void        setMaxSpeed(const float& speed);
    void        applyForce(const glm::vec3& force);
    static void avoidEdges(Boid& boid, const float& limit, const float& turnfactor, const float& protectedRadius);
    static void avoidElement(Boid& boid, const glm::vec3& position, const float& radius, const float& turnfactor);
    static void avoidMapElements(Boid& boid, const float& turnfactor);

    void limitSpeed(const float& maxSpeed);

    friend bool operator==(const Boid& a, const Boid& b);
    friend bool operator!=(const Boid& a, const Boid& b);
};