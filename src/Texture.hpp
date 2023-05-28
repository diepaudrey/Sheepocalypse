#pragma once
#include "p6/p6.h"
#include "vender/stb_image/stb_image.hpp"

class Texture {
private:
    unsigned int      m_RendererID;
    const img::Image& m_Image;

public:
    explicit Texture(img::Image img, unsigned int slot = 0);

    void        DeleteTexture();
    void        Bind(unsigned int slot = 0) const;
    static void UnBind(unsigned int slot = 0);

    inline int GetWidth() const { return m_Image.width(); }
    inline int GetHeight() const { return m_Image.height(); }
};