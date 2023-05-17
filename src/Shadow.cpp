#include "Shadow.hpp"

ShadowMapping::~ShadowMapping()
{
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_depthTexture);
}

bool ShadowMapping::InitWindow(unsigned int WindowWidth, unsigned int WindowHeight)
{
    m_width  = WindowWidth;
    m_height = WindowHeight;

    // Create the FBO
    glGenFramebuffers(1, &m_fbo);

    glGenTextures(1, &m_depthTexture);
    if (m_depthTexture == 0)
    {
        std::cout << "Failed to create the FBO" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float borderColor[] = {1.f, 0.f, 0.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("FB error", Status);
        return false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void ShadowMapping::BindForWriting()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height); // Width/height of shadow map
}

void ShadowMapping::BindForReading(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
}
void ShadowMapping::UnBind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowMapping::UseShader()
{
    m_shader.use();
}

void ShadowMapping::setShadow(glm::mat4 MVMatrix)
{
    // std::cout << "setShadow " << glm::value_ptr(MVMatrix) << std::endl;
    glUniformMatrix4fv(this->uMVPLight, 1, GL_FALSE, glm::value_ptr(MVMatrix));
}
