#pragma once
#include "RendererBoids.hpp"

class Mesh {
private:
    std::vector<glimac::ShapeVertex> m_vertices;
    Vbo                              m_vbo;
    Vao                              m_vao;
    std::vector<Texture>             m_textures;

    glm::mat4 m_modelMatrix;
    glm::vec3 m_position = glm::vec3(0.f);
    glm::vec3 m_rotation = glm::vec3(0.f);
    glm::vec3 m_scale    = glm::vec3(1.f);

    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 MVPMatrix;

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl");

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;
    GLuint m_uNumTextures;
    std::vector<GLuint>
        m_uTextures;

    // Fill the m_vertices, instead of copying them
    void InitVertexData(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices);

    void InitTextures(std::vector<Texture>& textures, const unsigned int& nbTextures);

    // Initialize vao
    void InitVao();

    void UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx);

    void InitUniforms();

    void UpdateUniforms();

public:
    Mesh(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices, std::vector<Texture>& textures, const unsigned int& nbTextures);

    ~Mesh();

    void Render(glm::mat4& viewMatrix, p6::Context& ctx);
};