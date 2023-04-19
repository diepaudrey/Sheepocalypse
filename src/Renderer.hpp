#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <utility>
#include <vector>
#include "Boid.hpp"
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

class Renderer {
public:
    std::vector<glimac::ShapeVertex> m_vertices;
    GLuint                           m_vbo;
    GLuint                           m_vao;

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl"); // à changer faire une classe shader
    GLuint     m_uMVPMatrix;
    GLuint     m_uMVMatrix;
    GLuint     m_uNormalMatrix;

public:
    Renderer();
    Renderer(std::vector<glimac::ShapeVertex>& vertices)
        : m_vertices(std::move(vertices))
    {
        initializeBoid();
    };
    void initializeBoid()
    {
        /*VBO*/
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glimac::ShapeVertex), m_vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /*VAO*/
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        static constexpr GLuint VERTEX_ATTR_POSITION  = 0;
        static constexpr GLuint VERTEX_ATTR_NORMAL    = 1;
        static constexpr GLuint VERTEX_ATTR_TEXCOORDS = 2;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

        glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)nullptr);

        glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));

        glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);

        /*Loading Shader*/
        // m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl");

        /*Location uniform variables*/
        m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

        m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

        m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");
    }

    void renderBoids(std::vector<Boid> m_boids, glm::mat4 viewMatrix, p6::Context& ctx)
    {
        // glm::mat4 viewMatrix   = camera.getViewMatrix();

        // glm::mat4 MVMatrix   = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
        // glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glimac::bind_default_shader();
        m_shader.use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 MVMatrix;
        glm::mat4 MVPMatrix;
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

        glBindVertexArray(m_vao);

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

        glBindVertexArray(0);
    };

    void deleteBuffers()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }
};