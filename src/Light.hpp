#pragma once
#include "cstddef"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Light {
public:
    const p6::Shader& m_shader;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

    GLint m_uKa;
    GLint m_uKd;
    GLint m_uKs;
    GLint m_uShininess;
    GLint m_uLightPos_vs;
    GLint m_uLightIntensity;

    Light(const p6::Shader& shader)
        : m_shader(shader)
    {
        uMVPMatrix        = glGetUniformLocation(this->m_shader.id(), "uMVPMatrix");
        uMVMatrix         = glGetUniformLocation(this->m_shader.id(), "uMVMatrix");
        uNormalMatrix     = glGetUniformLocation(this->m_shader.id(), "uNormalMatrix");
        m_uKa             = glGetUniformLocation(this->m_shader.id(), "uKa");
        m_uKd             = glGetUniformLocation(this->m_shader.id(), "uKd");
        m_uKs             = glGetUniformLocation(this->m_shader.id(), "uKs");
        m_uShininess      = glGetUniformLocation(this->m_shader.id(), "uShininess");
        m_uLightPos_vs    = glGetUniformLocation(this->m_shader.id(), "uLightPos_vs");
        m_uLightIntensity = glGetUniformLocation(this->m_shader.id(), "uLightIntensity");
    }
};
