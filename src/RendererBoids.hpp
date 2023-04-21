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
#include "glimac/Freefly.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
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
    Texture                          m_textureD{p6::load_image_buffer("assets/textures/Drake.jpg")};
    Texture                          m_textureL{p6::load_image_buffer("assets/textures/lila.png"), 1};
    Texture                          m_textureS{p6::load_image_buffer("assets/textures/salade.jpg")};

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl"); // à changer faire une classe shader
    GLuint     m_uMVPMatrix;
    GLuint     m_uMVMatrix;
    GLuint     m_uNormalMatrix;
    GLint      m_uTextureLila;
    GLint      m_uTextureDrake;

public:
    RendererBoids();
    RendererBoids(std::vector<glimac::ShapeVertex>& vertices)
        : m_vertices(std::move(vertices))
    {
        initializeBoid();
    };
    void initializeBoid()
    {
        this->m_vbo = Vbo(m_vertices.data(), m_vertices.size());
        m_vbo.UnBind();

        m_vao.AddBuffer(m_vbo);
        m_vao.UnBind();

        /*Location uniform variables*/
        m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

        m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

        m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

        m_uTextureDrake = glGetUniformLocation(m_shader.id(), "uTexture1");

        m_uTextureLila = glGetUniformLocation(m_shader.id(), "uTexture2");
    }

    void renderBoids(std::vector<Boid> m_boids, glm::mat4 viewMatrix, p6::Context& ctx)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glimac::bind_default_shader();
        m_shader.use();

        glm::mat4 MVMatrix;
        glm::mat4 MVPMatrix;
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 250.f);

        m_vao.Bind();
        // glUniform1i(m_uTextureSalade, 0);
        // m_textureS.Bind();

        // glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        // m_textureS.UnBind();

        glUniform1i(m_uTextureDrake, 0);
        glUniform1i(m_uTextureLila, 1);
        m_textureD.Bind();
        m_textureL.Bind(1);

        for (auto& boid : m_boids)
        {
            glm::vec3 start        = glm::vec3(0.f, 1.f, 0.f);
            glm::vec3 direction    = normalize(boid.getSpeed());
            glm::vec3 rotationAxis = glm::cross(start, direction);
            float     angle        = glm::orientedAngle(start, direction, start);

            MVMatrix = glm::translate(glm::mat4(1.f), boid.getPosition() + boid.getSpeed());
            MVMatrix = glm::rotate(MVMatrix, angle, rotationAxis);

            glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;

            glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

            glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

            glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        }
        m_textureD.UnBind();
        m_textureL.UnBind(1);
        m_vao.UnBind();
    };

    void deleteBuffers()
    {
        m_textureD.DeleteTexture();
        m_textureL.DeleteTexture();
        m_vbo.DeleteVbo();
        m_vao.DeleteVao();
    }
};