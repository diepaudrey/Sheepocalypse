#include "Texture.hpp"
#include "vender/stb_image/stb_image.hpp"

Texture::Texture(img::Image img)
    : m_Image(img)
{
    stbi_set_flip_vertically_on_load(1);
    // instead of bot left -> top left
    // m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Image.width(), m_Image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Image.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // if (m_LocalBuffer)
    // {
    //     stbi_image_free(m_LocalBuffer);
    // }
}

void Texture::DeleteTexture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
