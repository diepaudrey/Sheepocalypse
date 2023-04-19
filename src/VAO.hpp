#pragma once
#include "VBO.hpp"

class Vao {
private:
    unsigned int m_RendererID;

public:
    Vao();

    void AddBuffer(const Vbo& vbo);
    void Bind() const;
    void UnBind() const;
    void DeleteVao() const;
};