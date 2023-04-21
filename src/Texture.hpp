#pragma once
#include "p6/p6.h"

class Texture {
private:
    unsigned int      m_RendererID;
    const img::Image& m_Image;
    // unsigned char* m_LocalBuffer;

public:
    Texture() = default;
    Texture(img::Image img);

    void SetImage(img::Image img);
    void DeleteTexture();
    void Bind() const;
    void UnBind() const;

    inline int GetWidth() const { return m_Image.width(); }
    inline int GetHeight() const { return m_Image.height(); }
};