#include "Mesh.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"

Mesh::Mesh(std::vector<glimac::ShapeVertex>& vertices, std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, LightParams& lightP)
    : m_position(position), m_rotation(rotation), m_scale(scale)
{
    InitVertexData(vertices, vertices.size());
    InitVao();
    InitTextures(textures, textures.size());
    InitUniforms();
}

Mesh::Mesh(const Mesh& mesh)
    : m_position(mesh.m_position), m_rotation(mesh.m_rotation), m_scale(mesh.m_scale), m_vertices(mesh.m_vertices), m_textures(mesh.m_textures)
{
    InitVertexData(m_vertices, m_vertices.size());
    InitVao();
    InitTextures(m_textures, m_textures.size());
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

void Mesh::InitUniforms()
{
    m_uMVPMatrix    = glGetUniformLocation(m_shader.id(), "uMVPMatrix");
    m_uMVMatrix     = glGetUniformLocation(m_shader.id(), "uMVMatrix");
    m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");
    m_uNumTextures  = glGetUniformLocation(m_shader.id(), "uNumTextures");
    for (size_t i = 0; i < m_textures.size(); i++)
    {
        std::string name     = "uTextures[" + std::to_string(i) + "]";
        GLuint      location = glGetUniformLocation(m_shader.id(), name.c_str());
        m_uTextures.push_back(location);
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
    this->ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 10000.f);
    this->MVMatrix     = glm::translate(glm::mat4(1.f), m_position);
    MVMatrix           = glm::rotate(MVMatrix, m_rotation.x, glm::vec3(1.0f, 0.f, 0.f));
    MVMatrix           = glm::rotate(MVMatrix, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
    MVMatrix           = glm::rotate(MVMatrix, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
    MVMatrix           = glm::scale(MVMatrix, m_scale);
    this->NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    this->MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
}

void Mesh::UpdateMatricesMove(glm::mat4 viewMatrix, p6::Context& ctx)
{
    this->ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 10000.f);
    this->MVMatrix     = glm::translate(glm::mat4(1.f), m_position);
    MVMatrix           = glm::rotate(MVMatrix, m_rotation.x, glm::vec3(1.0f, 0.f, 0.f));
    MVMatrix           = glm::rotate(MVMatrix, m_rotation.y, glm::vec3(0.f, 1.f, 0.f));
    MVMatrix           = glm::rotate(MVMatrix, m_rotation.z, glm::vec3(0.f, 0.f, 1.f));
    MVMatrix           = glm::scale(MVMatrix, m_scale);
    this->NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    this->MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
}

void Mesh::UpdateUniforms()
{
    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glUniform1i(m_uNumTextures, m_textures.size());
}

void Mesh::BindTextures()
{
    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        glUniform1i(m_uTextures[i], i);
        m_textures[i].Bind(i);
    }
}

void Mesh::UnBindTextures()
{
    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        m_textures[i].UnBind(i);
    }
}

void Mesh::UpdatePosRot(glm::vec3& position, glm::vec3& rotation)
{
    m_position = position;
    m_rotation = rotation;
}

void Mesh::Render(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    m_shader.use();
    UpdateMatrices(viewMatrix, ctx);
    UpdateUniforms();
    BindTextures();
    lightMesh.setLight(lightMesh, lightP.light, MVMatrix, MVPMatrix);
    BasicRender();
    UnBindTextures();
}

void Mesh::RenderMoving(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP, glm::vec3& position, glm::vec3& rotation)
{
    m_shader.use();
    UpdatePosRot(position, rotation);
    UpdateMatricesMove(viewMatrix, ctx);
    UpdateUniforms();
    BindTextures();
    lightMesh.setLight(lightMesh, lightP.light, MVMatrix, MVPMatrix);
    BasicRender();
    UnBindTextures();
}

void Mesh::BasicRender()
{
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    m_vao.UnBind();
}

Mesh::~Mesh()
{
    m_vao.DeleteVao();
    m_vbo.DeleteVbo();
}

Mesh& Mesh::operator()(std::vector<glimac::ShapeVertex>& vertices, std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, LightParams& lightP)
{
    m_vertices = vertices;
    m_position = position;
    m_rotation = rotation;
    m_scale    = scale;
    m_textures.clear();
    for (int i = 0; i < textures.size(); i++)
    {
        Texture texture = textures[i];
        m_textures.push_back(texture);
    }
    InitVertexData(m_vertices, m_vertices.size());
    InitVao();
    InitTextures(m_textures, m_textures.size());
    lightMesh.initLight(lightP);
    InitUniforms();

    return *this;
}