#include "VAO.hpp"
#include "RendererBoids.hpp"

Vao::Vao()
{
    glGenVertexArrays(1, &m_RendererID);
    glBindVertexArray(m_RendererID);
}

void Vao::DeleteVao() const
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void Vao::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void Vao::UnBind() const
{
    glBindVertexArray(0);
}

void Vao::AddBuffer(const Vbo& vb)
{
    Bind();
    vb.Bind();
    static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
    static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
    static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)nullptr);
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));
}
