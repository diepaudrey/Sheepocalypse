#include "RendererBoids.hpp"

RendererBoids::RendererBoids(std::vector<glimac::ShapeVertex>& vertices)
    : m_vertices(std::move(vertices))
{
    initializeBoid();
}

void RendererBoids::initializeBoid()
{
    this->m_vbo = Vbo(m_vertices.data(), m_vertices.size());
    m_vbo.UnBind();

    m_vao.AddBuffer(m_vbo);
    m_vao.UnBind();

    /*Location uniform variables*/
    m_uMVPMatrix    = glGetUniformLocation(m_shader.id(), "uMVPMatrix");
    m_uMVMatrix     = glGetUniformLocation(m_shader.id(), "uMVMatrix");
    m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

    // m_uTextureDrake = glGetUniformLocation(m_shader.id(), "uTexture1");
    // m_uTextureLila = glGetUniformLocation(m_shader.id(), "uTexture2");
}

void RendererBoids::renderBoids(std::vector<Boid> m_boids, glm::mat4 viewMatrix, p6::Context& ctx)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glimac::bind_default_shader();
    m_shader.use();
    glm::vec3 uLightPos   = glm::vec4(light, 1);
    glm::vec3 uMVLightPos = glm::vec3(viewMatrix * glm::vec4(uLightPos, 1));

    glm::mat4 MVMatrix;
    glm::mat4 MVPMatrix;
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 250.f);

    m_vao.Bind();

    // glUniform1i(m_uTextureDrake, 0);
    // glUniform1i(m_uTextureLila, 1);
    // m_textureD.Bind();
    // m_textureL.Bind(1);
    _uKa.assign(50, white);
    _uKd.assign(50, white);
    _uKs.assign(50, white);
    _uShininess.assign(50, shininess);
    std::cout << "Light position: " << uMVLightPos.x << ", " << uMVLightPos.y << ", " << uMVLightPos.z << std::endl;
    for (int i = 0; i < 32; i++)
    {
        glm::vec3 start        = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 direction    = normalize(m_boids[i].getSpeed());
        glm::vec3 rotationAxis = glm::cross(start, direction);
        float     angle        = glm::orientedAngle(start, direction, start);

        MVMatrix = glm::translate(glm::mat4(1.f), m_boids[i].getPosition() + m_boids[i].getSpeed());
        MVMatrix = glm::rotate(MVMatrix, angle, rotationAxis);

        glm::mat4 NormalMatrix_light = glm::transpose(glm::inverse(MVMatrix));
        MVPMatrix                    = ProjMatrix * viewMatrix * MVMatrix;

        glUniformMatrix4fv(light_boid.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        glUniformMatrix4fv(light_boid.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(light_boid.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix_light));

        glUniform3fv(light_boid.m_uKa, 1, glm::value_ptr(_uKa[i]));
        glUniform3fv(light_boid.m_uKd, 1, glm::value_ptr(_uKd[i]));
        glUniform3fv(light_boid.m_uKs, 1, glm::value_ptr(_uKs[i]));
        glUniform1f(light_boid.m_uShininess, _uShininess[i]);

        // std::cout << "_uKa[" << i << "] = " << _uKa[i].r << ", " << _uKa[i].g << ", " << _uKa[i].b << std::endl;
        // std::cout << "_uKd[" << i << "] = " << _uKd[i].r << ", " << _uKd[i].g << ", " << _uKd[i].b << std::endl;
        // std::cout << "_uKs[" << i << "] = " << _uKs[i].r << ", " << _uKs[i].g << ", " << _uKs[i].b << std::endl;
        // std::cout << "_uShininess[" << i << "] = " << _uShininess[i] << std::endl;

        glUniform3fv(light_boid.m_uLightPos_vs, 1, glm::value_ptr(uMVLightPos));
        glm::vec3 lightIntensity = glm::vec3(10.0, 10.0, 10.0);
        glUniform3fv(light_boid.m_uLightIntensity, 1, glm::value_ptr(lightIntensity));

        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    }
    // m_textureD.UnBind();
    // m_textureL.UnBind(1);
    m_vao.UnBind();
};

void RendererBoids::deleteBuffers()
{
    // m_textureD.DeleteTexture();
    // m_textureL.DeleteTexture();
    m_vbo.DeleteVbo();
    m_vao.DeleteVao();
}