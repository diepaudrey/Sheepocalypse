#pragma once

#include <glm/gtx/vector_angle.hpp>
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
    // glm::vec3 m_position;
    // glm::vec3 m_rotation;
    // glm::vec3 m_scale;

    glm::vec3 m_position = glm::vec3(0.f);
    glm::vec3 m_rotation = glm::vec3(0.f);
    glm::vec3 m_scale    = glm::vec3(1.f);

    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;
    glm::mat4 MVPMatrix;

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl"); // à changer faire une classe shader
    Texture    m_textureD{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Diffuse.png")};
    // Texture    m_textureH{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Height.png"), 1};
    //  Texture    m_textureN{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Normal.png"),2};
    //  Texture    m_textureS{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Specular.png"),3};

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;
    GLuint m_uTexture;

    // Fill the m_vertices, instead of copying them
    void InitVertexData(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices)
    {
        for (size_t i = 0; i < nbVertices; i++)
        {
            this->m_vertices.push_back(vertices[i]);
        }
    }

    // Initialize vao
    void InitVao()
    {
        m_vao;
        m_vbo = Vbo(m_vertices.data(), m_vertices.size());
        m_vao.AddBuffer(m_vbo);
        m_vbo.UnBind();
    }

    void UpdateMatrices(glm::mat4 viewMatrix, p6::Context& ctx)
    {
        this->ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        this->MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
        this->NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        this->MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
    }

    void InitUniforms()
    {
        m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

        m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

        m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

        m_uTexture = glGetUniformLocation(m_shader.id(), "uTexture");
    }

    void UpdateUniforms()
    {
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    }

public:
    Mesh(std::vector<glimac::ShapeVertex> vertices, const unsigned int& nbVertices)
    {
        InitVertexData(vertices, nbVertices);
        InitVao();
        InitUniforms();
    }

    ~Mesh()
    {
        m_vao.UnBind();
        m_vbo.UnBind();
    }

    void update()
    {
    }

    void Render(glm::mat4& viewMatrix, p6::Context& ctx)
    {
        UpdateMatrices(viewMatrix, ctx);
        m_shader.use();
        UpdateUniforms();
        m_vao.Bind();
        glUniform1i(m_uTexture, 0);
        m_textureD.Bind();
        // m_textureD.Bind();
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
        m_textureD.UnBind();
        m_vao.UnBind();
    }
};