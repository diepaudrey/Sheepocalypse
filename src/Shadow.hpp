#pragma once
#include <vector>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class ShadowMapping {
public:
    int               m_width        = 0;
    int               m_height       = 0;
    GLuint            m_fbo          = 0;
    GLuint            m_depthTexture = 0;
    const p6::Shader& m_shader;
    GLint             uMVPLight;

public:
    explicit ShadowMapping(const p6::Shader& shader)
        : m_shader(shader)
    {
        this->uMVPLight = glGetUniformLocation(m_shader.id(), "depthMVP");
    };
    ~ShadowMapping();

    bool InitWindow(unsigned int WindowWidth, unsigned int WindowHeight);

    void        BindForWriting() const;
    void        BindForReading(GLenum TextureUnit) const;
    static void UnBind(GLenum TextureUnit);
    void        UseShader();
    void        SetShadow(glm::mat4 MVMatrix) const;
};
