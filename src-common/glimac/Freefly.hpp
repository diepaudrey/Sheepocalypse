#pragma once

#include <iostream>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"

namespace glimac {

class FreeflyCamera {
private:
    glm::vec3 m_position;
    float     m_phi;
    float     m_theta;
    glm::vec3 m_frontVector;
    glm::vec3 m_leftVector;
    glm::vec3 m_upVector;
    bool      m_paused;

    void ComputeDirectionVectors()
    {
        m_frontVector.x = cos(glm::radians(m_phi)) * cos(glm::radians(m_theta));
        m_frontVector.y = sin(glm::radians(m_theta));
        m_frontVector.z = sin(glm::radians(m_phi)) * cos(glm::radians(m_theta));
        m_frontVector   = glm::normalize(m_frontVector);

        m_upVector.x = -cos(glm::radians(m_phi)) * sin(glm::radians(m_theta));
        m_upVector.y = cos(glm::radians(m_theta));
        m_upVector.z = -sin(glm::radians(m_phi)) * sin(glm::radians(m_theta));
        m_upVector   = glm::normalize(m_upVector);

        m_leftVector = glm::cross(m_frontVector, m_upVector);
        m_leftVector = glm::normalize(m_leftVector);
    }

    bool CamIsOutBorders(const float& limit) const
    {
        bool exp = m_position.x > limit || m_position.y > limit || m_position.z > limit || m_position.x < -limit || m_position.y < -limit || m_position.z < -limit;

        if (exp)
        {
            std::cout << "Player is out borders " << std::endl;
        }
        return exp;
    }

public:
    FreeflyCamera()
        : m_position(glm::vec3(-20.f, 0.f, -20.f)), m_phi(glm::pi<float>()), m_theta(0.f), m_paused(false)
    {
        ComputeDirectionVectors();
    };

    glm::vec3 GetPosition()
    {
        return m_position;
    }

    glm::vec3 GetUpVector()
    {
        return m_upVector;
    }

    glm::vec3 GetLeftVector()
    {
        return m_leftVector;
    }

    void MoveLeft(float t)
    {
        if (!m_paused)
        {
            m_position -= t * m_leftVector;
            ComputeDirectionVectors();
        }
    }

    void MoveFront(float t)
    {
        if (!m_paused)
        {
            m_position += t * m_frontVector;
            ComputeDirectionVectors();
        }
    }

    void RotateLeft(float degrees)
    {
        if (!m_paused)
        {
            m_phi += glm::radians(degrees);
            ComputeDirectionVectors();
        }
    }

    void RotateUp(float degrees)
    {
        if (!m_paused)
        {
            m_theta += -glm::radians(degrees);
            ComputeDirectionVectors();
        }
    }

    void TogglePause()
    {
        m_paused = !m_paused;
    }

    bool IsPaused() const
    {
        return m_paused;
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_frontVector, m_upVector);
    }

    bool FixCamLimit(const float& limit)
    {
        if (m_position.x > limit)
        {
            m_position.x = limit - 1;
            return true;
        }
        if (m_position.x < -limit)
        {
            m_position.x = -limit + 1;
            return true;
        }
        if (m_position.y > limit)
        {
            m_position.y = limit - 1;
            return true;
        }
        if (m_position.y < -limit + 200)
        {
            m_position.y = -limit + 201;
            return true;
        }
        if (m_position.z > limit)
        {
            m_position.z = limit - 1;
            return true;
        }
        if (m_position.z < -limit)
        {
            m_position.z = -limit + 1;
            return true;
        }
        return false;
    }
};

} // namespace glimac