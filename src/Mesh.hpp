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

    p6::Shader    m_shader       = p6::load_shader("shaders/3D.vs.glsl", "shaders/LightAndText.fs.glsl");
    p6::Shader    m_lightshader  = p6::load_shader("shaders/3D.vs.glsl", "shaders/directionalLight.fs.glsl");
    p6::Shader    m_shadowShader = p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl");
    ShadowMapping m_shadowMap    = p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl");
    Light         lightMesh{m_shader};
    Light         lightMesh2{m_shader};

    GLuint              m_uMVPMatrix;
    GLuint              m_uMVMatrix;
    GLuint              m_uNormalMatrix;
    GLuint              m_uNumTextures;
    std::vector<GLuint> m_uTextures;

    // Fill the m_vertices, instead of copying them
    void InitVertexData(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices);
    void InitTextures(std::vector<Texture>& textures, const unsigned int& nbTextures);
    void InitUniforms();

    // Initialize vao
    void InitVao();

    void UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx);
    void UpdateMatricesMove(glm::mat4 viewMatrix, p6::Context& ctx);

    void InitShadow(const glm::vec3& lightPos);

    void UpdateUniforms();
    void BindTextures();
    void UnBindTextures();
    void UpdatePosRot(glm::vec3& position, glm::vec3& rotation);

public:
    Mesh() = default;
    Mesh(std::vector<glimac::ShapeVertex>& vertices, std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, LightParams& lightP);
    Mesh(const Mesh& mesh);
    ~Mesh();

    void Render(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);
    void RenderShadow(const glm::vec3& lightPos);
    void RenderMoving(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP, glm::vec3& position, glm::vec3& rotation);
    void ShadowMapPass(glm::mat4& viewMatrix, const glm::vec3& lightPos);
    void BasicRender();

    Mesh&
        operator()(std::vector<glimac::ShapeVertex>& vertices, std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, LightParams& lightP);
};