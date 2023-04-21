#include "VBO.hpp"
#include "RendererBoids.hpp"

Vbo::Vbo(const void* data, unsigned int size)
{
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(glimac::ShapeVertex), data, GL_STATIC_DRAW);
}
void Vbo::DeleteVbo() const
{
    glDeleteBuffers(1, &m_RendererID);
}

void Vbo::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}
void Vbo::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}