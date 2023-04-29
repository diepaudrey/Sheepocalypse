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

    void computeDirectionVectors()
    {
        // m_frontVector
        m_frontVector.x = cos(glm::radians(m_phi)) * cos(glm::radians(m_theta));
        m_frontVector.y = sin(glm::radians(m_theta));
        m_frontVector.z = sin(glm::radians(m_phi)) * cos(glm::radians(m_theta));

        m_frontVector = glm::normalize(m_frontVector);

        // m_leftVector
        // m_leftVector.x = sin(glm::radians(m_phi + glm::half_pi<float>()));
        // m_leftVector.y = 0.f;
        // m_leftVector.z = cos(glm::radians(m_phi + glm::half_pi<float>()));

        // m_upVector
        // m_upVector = glm::cross(m_frontVector, m_leftVector);

        m_upVector.x = -cos(glm::radians(m_phi)) * sin(glm::radians(m_theta));
        m_upVector.y = cos(glm::radians(m_theta));
        m_upVector.z = -sin(glm::radians(m_phi)) * sin(glm::radians(m_theta));
        m_upVector   = glm::normalize(m_upVector);

        m_leftVector = glm::cross(m_frontVector, m_upVector);
        m_leftVector = glm::normalize(m_leftVector);
    }

    bool camIsOutBorders(const float& limit)
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
        computeDirectionVectors();
    };

    glm::vec3 getPosition()
    {
        return m_position;
    }

    glm::vec3 getUpVector()
    {
        return m_upVector;
    }

    glm::vec3 getLeftVector()
    {
        return m_leftVector;
    }

    void moveLeft(float t)
    {
        //  glm::vec3 direction = glm::normalize(glm::cross(m_frontVector, m_upVector));
        if (!m_paused)
        {
            m_position -= t * m_leftVector;
            computeDirectionVectors();
        }
    }

    void moveFront(float t)
    {
        if (!m_paused)
        {
            m_position += t * m_frontVector;
            computeDirectionVectors();
        }
    }

    void rotateLeft(float degrees)
    {
        if (!m_paused)
        {
            m_phi += glm::radians(degrees);
            computeDirectionVectors();
        }
    }

    void rotateUp(float degrees)
    {
        if (!m_paused)
        {
            m_theta += -glm::radians(degrees);
            computeDirectionVectors();
        }
    }

    void togglePause()
    {
        m_paused = !m_paused;
    }

    bool isPaused() const
    {
        return m_paused;
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_frontVector, m_upVector);
    }

    void fixCamLimit(const float& limit)
    {
        if (m_position.x > limit)
        {
            m_position.x = limit;
        }
        else if (m_position.x < -limit)
        {
            m_position.x = -limit;
        }
        else if (m_position.y > limit)
        {
            m_position.y = limit;
        }
        else if (m_position.y < -limit)
        {
            m_position.y = -limit;
        }
        else if (m_position.z > limit)
        {
            m_position.z = limit;
        }
        else if (m_position.z < -limit)
        {
            m_position.z = -limit;
        }
    }
};

} // namespace glimac