#include "Mesh.hpp"

Mesh::Mesh(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices)
{
    InitVertexData(vertices, nbVertices);
    InitVao();
    InitUniforms();
}
Mesh::~Mesh()
{
    m_vao.UnBind();
    m_vbo.UnBind();
}

void Mesh::render(glm::mat4& viewMatrix, p6::Context& ctx)
{
    UpdateMatrices(viewMatrix, ctx);
    m_shader.use();
    UpdateUniforms();
    m_vao.Bind();
    glEnable(GL_DEPTH_TEST);
    // glUniform1i(m_uTexture, 0);
    // m_texture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    // m_texture.UnBind();
    m_vao.UnBind();
}

void Mesh::InitVertexData(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices)
{
    for (size_t i = 0; i < nbVertices; i++)
    {
        this->m_vertices.push_back(vertices[i]);
    }
}

void Mesh::InitVao()
{
    m_vao;
    m_vbo = Vbo(m_vertices.data(), m_vertices.size());
    m_vao.AddBuffer(m_vbo);
    m_vbo.UnBind();
}

void Mesh::UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx)
{
    this->ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    this->MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
    this->NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    this->MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
}

void Mesh::InitUniforms()
{
    m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

    m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

    m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

    m_uTexture = glGetUniformLocation(m_shader.id(), "uTexture");
}

void Mesh::UpdateUniforms()
{
    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}
