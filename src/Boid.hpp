#pragma once
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_position;
    glm::vec3 m_speed;

public:
    Boid() = default;
    Boid(const glm::vec3& pos, const glm::vec3 vel)
        : m_position(pos), m_speed(vel){};

    glm::vec3 GetPosition() const
    {
        return m_position;
    }
    glm::vec3 GetSpeed() const
    {
        return m_speed;
    }

    void        UpdatePosition(p6::Context& ctx);
    void        SetMaxSpeed(const float& speed);
    void        ApplyForce(const glm::vec3& force);
    static void AvoidEdges(Boid& boid, const float& limit, const float& turnfactor, const float& protectedRadius);
    static void AvoidElement(Boid& boid, const glm::vec3& position, const float& radius, const float& turnfactor);
    static void AvoidMapElements(Boid& boid, const float& turnfactor);

    void LimitSpeed(const float& maxSpeed);

    friend bool operator==(const Boid& a, const Boid& b);
    friend bool operator!=(const Boid& a, const Boid& b);
};