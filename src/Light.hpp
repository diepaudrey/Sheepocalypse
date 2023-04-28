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
    GLint m_uLightColor;

    glm::vec3              m_lightIntensity;
    std::vector<glm::vec3> _uKa;
    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;

public:
    Light(const p6::Shader& shader);
    void initLight(const glm::vec3 Ka, const glm::vec3 Kd, const glm::vec3 Ks, const float shininess, glm::vec3 lightIntensity);
    void setLight(Light light, glm::vec3 posLight, glm::mat4 MVMatrix, glm::mat4 MVPMatrix);
};