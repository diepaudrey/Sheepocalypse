#include "RendererBoids.hpp"
#include "glm/ext/matrix_transform.hpp"

RendererBoids::RendererBoids(std::vector<glimac::ShapeVertex>& vertices)
    : m_vertices(std::move(vertices))
{
    initializeBoid();
}

void RendererBoids::InitVao()
{
    this->m_vbo = Vbo(m_vertices.data(), m_vertices.size());
    m_vbo.UnBind();
    m_vao.AddBuffer(m_vbo);
    m_vao.UnBind();
}

void RendererBoids::InitTextures()
{
    m_uNumTextures = glGetUniformLocation(m_shader.id(), "uNumTextures");
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        std::string name     = "uTextures[" + std::to_string(i) + "]";
        GLuint      location = glGetUniformLocation(m_shader.id(), name.c_str());
        m_uTextures.push_back(location);
    }
}

void RendererBoids::BindTexture()
{
    glUniform1i(m_uNumTextures, m_textures.size());
    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        glUniform1i(m_uTextures[i], i);
        m_textures[i].Bind(i);
    }
}

void RendererBoids::UnBindTexture()
{
    for (unsigned int i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i].UnBind(i);
    }
}

void RendererBoids::initializeBoid()
{
    InitVao();
    InitTextures();
    light_boid.initLight(Ka, Kd, Ks, shininess);
}

void RendererBoids::renderBoids(std::vector<Boid> m_boids, glm::mat4 viewMatrix, p6::Context& ctx)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glimac::bind_default_shader();
    m_shader.use();

    glm::mat4 MVMatrix;
    glm::mat4 MVPMatrix;
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 500.f);

    m_vao.Bind();
    BindTexture();

    for (auto& boid : m_boids)
    {
        glm::vec3 start        = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 direction    = normalize(boid.getSpeed());
        glm::vec3 rotationAxis = glm::cross(start, direction);
        float     angle        = glm::orientedAngle(start, direction, start);

        MVMatrix  = glm::translate(glm::mat4(1.f), boid.getPosition() + boid.getSpeed());
        MVMatrix  = glm::rotate(MVMatrix, angle, rotationAxis);
        MVMatrix  = glm::scale(MVMatrix, glm::vec3(2.0f));
        MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;

        light_boid.setLight(light_boid, light, MVMatrix, MVPMatrix);
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    }

    UnBindTexture();
    m_vao.UnBind();
};

void RendererBoids::deleteBuffers()
{
    m_vbo.DeleteVbo();
    m_vao.DeleteVao();
}