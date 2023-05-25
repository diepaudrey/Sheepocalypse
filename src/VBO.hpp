#pragma once

class Vbo {
private:
    unsigned int m_RendererID;

public:
    Vbo() = default;
    Vbo(const void* data, unsigned int size);

    void        Bind() const;
    static void UnBind();
    void        DeleteVbo() const;
};