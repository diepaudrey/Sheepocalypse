#include "Light.hpp"

Light::Light(const p6::Shader& shader)
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

void Light::initLight(LightParams light)
{
    _uKa.emplace_back(light.Ka);
    _uKd.emplace_back(light.Kd);
    _uKs.emplace_back(light.Ks);
    _uShininess.push_back(light.shininess);
    m_lightIntensity = light.lightIntensity;
}

void Light::setLight(const Light& light_boid, glm::vec3 posLight, glm::mat4 MVMatrix, glm::mat4 MVPMatrix)
{
    glm::vec3 uLightPos          = glm::vec4(posLight, 1);
    glm::mat4 NormalMatrix_light = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(light_boid.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
    glUniformMatrix4fv(light_boid.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(light_boid.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix_light));

    glUniform3fv(light_boid.m_uKa, 1, glm::value_ptr(_uKa[0]));
    glUniform3fv(light_boid.m_uKd, 1, glm::value_ptr(_uKd[0]));
    glUniform3fv(light_boid.m_uKs, 1, glm::value_ptr(_uKs[0]));
    glUniform1f(light_boid.m_uShininess, _uShininess[0]);

    glUniform3fv(light_boid.m_uLightPos_vs, 1, glm::value_ptr(uLightPos));
    glUniform3fv(light_boid.m_uLightIntensity, 1, glm::value_ptr(m_lightIntensity));
}
