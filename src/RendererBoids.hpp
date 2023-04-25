#pragma once
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <utility>
#include <vector>
#include "Boid.hpp"
#include "Light.hpp"
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
    // Texture                          m_textureD{p6::load_image_buffer("assets/textures/Drake.jpg")};
    // Texture                          m_textureL{p6::load_image_buffer("assets/textures/lila.png"), 1};

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/pointLight.fs.glsl"); // à changer faire une classe shader
    Light      light_boid{p6::load_shader("shaders/3D.vs.glsl", "shaders/pointLight.fs.glsl")};
    GLuint     m_uMVPMatrix;
    GLuint     m_uMVMatrix;
    GLuint     m_uNormalMatrix;
    // GLint                  m_uTextureLila;
    // GLint                  m_uTextureDrake;
    glm::mat4              MVMatrix_light;
    glm::mat4              NormalMatrix_light;
    std::vector<glm::vec3> _uKa;
    std::vector<glm::vec3> _uKd;
    std::vector<glm::vec3> _uKs;
    std::vector<float>     _uShininess;
    glm::vec3              light     = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3              white     = glm::vec3(1.0f, 1.0f, 1.0f);
    float                  shininess = 32.0f;

public:
    RendererBoids();

    RendererBoids(std::vector<glimac::ShapeVertex>& vertices);

    void initializeBoid();

    void renderBoids(std::vector<Boid> m_boids, glm::mat4 viewMatrix, p6::Context& ctx);

    void deleteBuffers();
};