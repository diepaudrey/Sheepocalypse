#pragma once

#include <stddef.h>
#include <utility>
#include "cstddef"
#include "glimac/Freefly.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Boid {
public:
    std::vector<glimac::ShapeVertex> m_vertices;
    GLuint                           m_vbo;
    GLuint                           m_vao;

    int index;

public:
    Boid();
    Boid(std::vector<glimac::ShapeVertex> vertices, const GLuint& vbo, const GLuint& vao)
        : m_vertices(std::move(vertices)), m_vbo(vbo), m_vao(vao){};
    // ~Boid();

    void initializeBoid()
    {
        /*VBO*/
        // GLuint vbo;
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        // vertices = glimac::cone_vertices(1.f, 0.5f, 5, 5);

        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glimac::ShapeVertex), m_vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /*VAO*/
        // GLuint vao;
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
    }

    void drawBoid(const p6::Shader& shader, glimac::FreeflyCamera& camera, p6::Context& ctx, GLuint uMVPMatrix, GLuint uMVMatrix, GLuint uNormalMatrix)
    {
        glm::mat4 viewMatrix   = camera.getViewMatrix();
        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glimac::bind_default_shader();
        shader.use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 MVPMatrix = ProjMatrix * viewMatrix * MVMatrix;
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(m_vao);

        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

        glBindVertexArray(0);
    }
};