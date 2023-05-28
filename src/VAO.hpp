#pragma once

#include "VBO.hpp"
#include "glimac/common.hpp"
#include "p6/p6.h"


class Vao {
private:
    unsigned int m_RendererID;

public:
    Vao();

    void        AddBuffer(const Vbo& vbo) const;
    void        Bind() const;
    static void UnBind();
    void        DeleteVao() const;
};