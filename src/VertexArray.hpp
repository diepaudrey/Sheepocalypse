#pragma once
#include "VertexBuffer.hpp"

class VertexArray {
private:
    unsigned int m_RendererID;

public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vbo);
    void Bind() const;
    void UnBind() const;
};