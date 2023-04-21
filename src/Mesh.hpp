#pragma once

#include <glm/gtx/vector_angle.hpp>
#include <vector>
#include "Boid.hpp"
#include "Light.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "cstddef"
#include "glimac/Freefly.hpp"
#include "glimac/common.hpp"
#include "glimac/cone_vertices.hpp"
#include "glimac/default_shader.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Mesh {
private:
    std::vector<glimac::ShapeVertex> m_vertices;
    Vbo                              m_vbo;
    Vao                              m_vao;

    glm::mat4 m_modelMatrix;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 MVPMatrix;

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/normals.fs.glsl"); // à changer faire une classe shader
    GLuint     m_uMVPMatrix;
    GLuint     m_uMVMatrix;
    GLuint     m_uNormalMatrix;

    void InitVertexData(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices)
    {
        for (size_t i = 0; i < nbVertices; i++)
        {
            this->m_vertices.push_back(vertices[i]);
        }
    }

    void InitVao()
    {
        m_vao;
        m_vbo = Vbo(m_vertices.data(), m_vertices.size());
        m_vao.AddBuffer(m_vbo);
        m_vbo.UnBind();
    }

    void InitModelMatrix(p6::Context& ctx)
    {
        m_position    = glm::vec3(0.f);
        m_rotation    = glm::vec3(0.f);
        m_scale       = glm::vec3(1.f);
        m_modelMatrix = glm::mat4(1.f);

        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

        ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    }

    void UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx)
    {
        MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
    }

    void InitUniforms()
    {
        m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

        m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

        m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");
    }

    void UpdateUniforms()
    {
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }

public:
    Mesh(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices, p6::Context& ctx)
    {
        InitVertexData(vertices, nbVertices);
        InitVao();
        InitUniforms();
        InitModelMatrix(ctx);
    }

    ~Mesh()
    {
        m_vao.UnBind();
        m_vbo.UnBind();
    }

    void update()
    {
    }

    void render(glm::mat4& viewMatrix, p6::Context& ctx)
    {
        UpdateMatrices(viewMatrix, ctx);
        // glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        // glm::mat4 MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, -2.f, 0.f));
        // glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        // glm::mat4 MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
        m_shader.use();
        // // UpdateUniforms();
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        m_vao.Bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        m_vao.UnBind();

        // glm::mat4 modelViewMatrix = viewMatrix * m_modelMatrix;
        // glm::mat4 normalMatrix    = glm::transpose(glm::inverse(modelViewMatrix));

        // glm::mat4 MVPMatrix = ProjMatrix * modelViewMatrix;

        // m_shader.use();
        // glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
        // glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        // glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        // m_vao.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        // m_vao.UnBind();
    }
};