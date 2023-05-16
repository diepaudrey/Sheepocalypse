#pragma once
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include "Boid.hpp"
#include "Light.hpp"
#include "Shadow.hpp"
#include "Texture.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "cstddef"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class RendererBoids {
public:
    std::vector<glimac::ShapeVertex> m_vertices;
    Vbo                              m_vbo;
    Vao                              m_vao;
    p6::Shader                       m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/LightAndText.fs.glsl");
    p6::Shader                       m_shadow = p6::load_shader("shaders/shadow.vs.glsl", "shaders/shadow.fs.glsl");

    // parameters for text
    Texture              m_textureD{p6::load_image_buffer("assets/textures/body.png")};
    Texture              m_textureD2{p6::load_image_buffer("assets/textures/body2.png"), 1};
    Texture              m_textureW{p6::load_image_buffer("assets/textures/wings.png"), 2};
    Texture              m_textureL{p6::load_image_buffer("assets/textures/lila.png"), 3};
    std::vector<Texture> m_textures = {m_textureD, m_textureD2, m_textureW, m_textureL};
    GLuint               m_uNumTextures;
    std::vector<GLuint>  m_uTextures;

    // parameters for light
    Light         light_boid{m_shader};
    ShadowMapping shadow_boid{m_shadow};

public:
    RendererBoids() = default;

    RendererBoids(std::vector<glimac::ShapeVertex>& vertices, LightParams& lightP);

    void InitVao();

    void InitTextures();

    void initializeBoid(LightParams& lightP);

    void BindTexture();

    void UnBindTexture();

    void renderBoids(std::vector<Boid> m_boids, glm::mat4 viewMatrix, p6::Context& ctx, LightParams& lightP);

    void deleteBuffers();
};