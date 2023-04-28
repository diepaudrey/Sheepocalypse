#pragma once
#include "RendererBoids.hpp"

class Mesh {
private:
    std::vector<glimac::ShapeVertex> m_vertices;
    Vbo                              m_vbo;
    Vao                              m_vao;
    std::vector<Texture>             m_textures;

    glm::mat4 m_modelMatrix;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 MVPMatrix;

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/LightAndText.fs.glsl");

    // parameters for light
    Light lightMesh{m_shader};
    // LightParams m_lightP;
    //  glm::vec3       light          = glm::vec3(0.f, 50.f, 0.f);
    //  glm::vec3       lightIntensity = glm::vec3(5000.f, 5000.f, 5000.f);
    //  const glm::vec3 Ka             = glm::vec3(0.05, 0.05, 0.05);
    //  const glm::vec3 Kd             = glm::vec3(1.0, 1.0, 1.0);
    //  const glm::vec3 Ks             = glm::vec3(1.0, 1.0, 1.0);
    //  const float     shininess      = 0.5f;

    GLuint              m_uMVPMatrix;
    GLuint              m_uMVMatrix;
    GLuint              m_uNormalMatrix;
    GLuint              m_uNumTextures;
    std::vector<GLuint> m_uTextures;

    // Fill the m_vertices, instead of copying them
    void InitVertexData(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices);

    void InitTextures(std::vector<Texture>& textures, const unsigned int& nbTextures);

    // Initialize vao
    void InitVao();

    void UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx);

    void InitUniforms();

    void UpdateUniforms();

public:
    Mesh() = default;
    Mesh(std::vector<glimac::ShapeVertex>& vertices, std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, LightParams& lightP);
    Mesh(const Mesh& mesh);
    ~Mesh();

    void Render(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);

    // Mesh& operator=(const Mesh& mesh)
    // {
    //     m_position = mesh.m_position;
    //     m_rotation = mesh.m_rotation;
    //     m_scale    = mesh.m_scale;
    //     m_vertices = mesh.m_vertices;
    //     for (int i = 0; i < mesh.m_textures.size(); i++)
    //     {
    //         Texture texture = mesh.m_textures[i];
    //         m_textures.push_back(texture);
    //     }
    //     InitVertexData(m_vertices, m_vertices.size());
    //     InitVao();
    //     InitTextures(m_textures, m_textures.size());
    //     // lightMesh.initLight(m_lightP);
    //     InitUniforms();
    //     return *this;
    // }
    Mesh& operator()(std::vector<glimac::ShapeVertex>& vertices, std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, LightParams& lightP);
};