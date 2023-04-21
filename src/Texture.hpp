#pragma once
#include "p6/p6.h"

class Texture {
private:
    unsigned int      m_RendererID;
    const img::Image& m_Image;

public:
    Texture() = default;
    Texture(img::Image img, unsigned int slot = 0);

    void DeleteTexture();
    void Bind(unsigned int slot = 0) const;
    void UnBind(unsigned int slot = 0) const;

    inline int GetWidth() const { return m_Image.width(); }
    inline int GetHeight() const { return m_Image.height(); }
};