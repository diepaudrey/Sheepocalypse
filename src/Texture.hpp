#pragma once
#include "p6/p6.h"

class Texture {
private:
    unsigned int   m_RendererID;
    std::string    m_FilePath;
    unsigned char* m_LocalBuffer;
    int            m_Width, m_Height, m_BPP;

public:
    Texture() = default;
    explicit Texture(const std::string& path);
    ~Texture();

    void Bind() const;
    void UnBind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};