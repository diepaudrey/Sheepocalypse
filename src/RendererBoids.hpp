#pragma once
#include <glm/gtx/vector_angle.hpp>
#include "Boid.hpp"
#include "Light.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "p6/p6.h"

class RendererBoids {
public:
    std::vector<glimac::ShapeVertex> m_vertices;
    Vbo                              m_vbo;
    Vao                              m_vao;
    p6::Shader                       m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/LightAndText.fs.glsl");
    // p6::Shader                       m_shadow = p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl");

    Texture              m_textureD{p6::load_image_buffer("assets/textures/DrakeBoids.jpg")};
    Texture              m_textureS{p6::load_image_buffer("assets/textures/Shiny.jpg"), 1};
    std::vector<Texture> m_textures = {m_textureD, m_textureS};
    GLuint               m_uNumTextures;
    std::vector<GLuint>  m_uTextures;

    Light light_boid{m_shader};
    // ShadowMapping shadow_boid{m_shadow};

    void InitVao();
    void InitTextures();
    void InitializeBoid(LightParams& lightP);
    void BindTexture();
    void UnBindTexture();

public:
    RendererBoids() = default;
    RendererBoids(std::vector<glimac::ShapeVertex>& vertices, LightParams& lightP);

    void RenderBoids(const std::vector<Boid>& m_boids, glm::mat4 viewMatrix, p6::Context& ctx, LightParams& lightP);
    void DeleteBuffers();
};