#pragma once
#include <glm/gtx/vector_angle.hpp>
// #include <utility>
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
    Texture                          m_texture{p6::load_image_buffer("assets/textures/AUDREY.jpg")};

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl"); // à changer faire une classe shader
    GLuint     m_uMVPMatrix;
    GLuint     m_uMVMatrix;
    GLuint     m_uNormalMatrix;
    GLint      m_uTexture;

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

        m_uTexture = glGetUniformLocation(m_shader.id(), "uTexture");
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
        glUniform1i(m_uTexture, 0);
        m_texture.Bind();

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

        m_texture.UnBind();
        m_vao.UnBind();
    };

    void RenderBorders(const float& radius, glm::mat4 viewMatrix, p6::Context& ctx)
    {
        glm::vec3 A = {-radius, radius, -radius};
        glm::vec3 B = {radius, radius, -radius};
        glm::vec3 C = {radius, -radius, -radius};
        glm::vec3 D = {-radius, -radius, -radius};
        glm::vec3 E = {-radius, -radius, radius};
        glm::vec3 F = {-radius, radius, radius};
        glm::vec3 G = {radius, radius, radius};
        glm::vec3 H = {radius, -radius, radius};

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        std::vector<glm::vec3> vertices = {
            A, B, C, C, D, A,
            A, D, E, E, F, A,
            F, E, G, G, H, E,
            G, H, B, B, C, H};

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        // glBindVertexArray(vao);

        static constexpr GLuint vertex_attr_position = 0;
        glEnableVertexAttribArray(vertex_attr_position);

        static constexpr GLuint vertex_attr_color = 1;
        glEnableVertexAttribArray(vertex_attr_color);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindVertexArray(vao);

        glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

        glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glm::mat4 MVMatrix;
        // glm::mat4 MVPMatrix;
        // glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 250.f);
        // glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        // MVPMatrix              = ProjMatrix * viewMatrix * MVMatrix;

        // glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        // glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        // glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glimac::bind_default_shader();
        m_shader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // glDeleteBuffers(0, &vbo);
        // glDeleteVertexArrays(0, &vao);
    }

    void deleteBuffers()
    {
        m_texture.DeleteTexture();
        m_vbo.DeleteVbo();
        m_vao.DeleteVao();
    }
};