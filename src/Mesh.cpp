#include "Mesh.hpp"

Mesh::Mesh(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices, std::vector<Texture>& textures, const unsigned int& nbTextures)
{
    InitVertexData(vertices, nbVertices);
    InitVao();
    InitTextures(textures, nbTextures);
    InitUniforms();
}

void Mesh::InitVertexData(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices)
{
    for (size_t i = 0; i < nbVertices; i++)
    {
        this->m_vertices.push_back(vertices[i]);
    }
}

void Mesh::InitTextures(std::vector<Texture>& textures, const unsigned int& nbTextures)
{
    for (size_t i = 0; i < nbTextures; i++)
    {
        this->m_textures.push_back(textures[i]);
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
    this->ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 1000.f);
    this->MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
    this->NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    this->MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
}

void Mesh::InitUniforms()
{
    m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

    m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

    m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

    m_uNumTextures = glGetUniformLocation(m_shader.id(), "uNumTextures");

    // initialize each of the uniform variable needed
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        std::string name     = "uTextures[" + std::to_string(i) + "]";
        GLuint      location = glGetUniformLocation(m_shader.id(), name.c_str());
        m_uTextures.push_back(location);
    }
}

void Mesh::UpdateUniforms()
{
    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glUniform1i(m_uNumTextures, m_textures.size());
}

void Mesh::Render(glm::mat4& viewMatrix, p6::Context& ctx)
{
    m_shader.use();
    UpdateMatrices(viewMatrix, ctx);
    UpdateUniforms();
    m_vao.Bind();

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        glUniform1i(m_uTextures[i], i);
        m_textures[i].Bind(i);
    }

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i].UnBind(i);
    }
    m_vao.UnBind();
}

Mesh::~Mesh()
{
    m_vao.DeleteVao();
    m_vbo.DeleteVbo();
}