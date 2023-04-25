#pragma once
#include "RendererBoids.hpp"

class Mesh {
private:
    std::vector<glimac::ShapeVertex> m_vertices;
    Vbo                              m_vbo;
    Vao                              m_vao;

    glm::mat4 m_modelMatrix;
    glm::vec3 m_position = glm::vec3(0.f);
    glm::vec3 m_rotation = glm::vec3(0.f);
    glm::vec3 m_scale    = glm::vec3(1.f);

    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 MVPMatrix;

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl"); // à changer faire une classe shader
    Texture    m_texture{p6::load_image_buffer("assets/textures/AUDREY.jpg")};

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;
    GLuint m_uTexture;

    // Fill the m_vertices, instead of copying them
    void InitVertexData(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices);

    // Initialize vao
    void InitVao();

    void UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx);

    void InitUniforms();

    void UpdateUniforms();

public:
    Mesh(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices);

    ~Mesh();

    void render(glm::mat4& viewMatrix, p6::Context& ctx);
};