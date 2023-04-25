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
    std::vector<Texture>             m_textures;

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

    p6::Shader m_shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl");
    // Texture    m_texture{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Diffuse.png")};
    //   Texture    m_textureH{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Height.png"), 1};
    //    Texture    m_textureN{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Normal.png"),2};
    //    Texture    m_textureS{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Specular.png"),3};

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;
    GLuint m_uNumTextures;
    std::vector<GLuint>
        m_uTextures;

    // Fill the m_vertices, instead of copying them
    void InitVertexData(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices)
    {
        for (size_t i = 0; i < nbVertices; i++)
        {
            this->m_vertices.push_back(vertices[i]);
        }
    }

    void InitTextures(std::vector<Texture>& textures, const unsigned int& nbTextures)
    {
        for (size_t i = 0; i < nbTextures; i++)
        {
            this->m_textures.push_back(textures[i]);
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
        this->ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 1000.f);
        this->MVMatrix     = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -5.0f));
        this->NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        this->MVPMatrix    = ProjMatrix * viewMatrix * MVMatrix;
    }

    void InitUniforms()
    {
        m_uMVPMatrix = glGetUniformLocation(m_shader.id(), "uMVPMatrix");

        m_uMVMatrix = glGetUniformLocation(m_shader.id(), "uMVMatrix");

        m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

        m_uNumTextures = glGetUniformLocation(m_shader.id(), "uNumTextures");

        // initialize each of the uniform variable needed
        for (size_t i = 0; i < m_textures.size(); i++)
        {
            // std::string name     = "uTexture" + std::to_string(i + 1);
            // GLuint      location = glGetUniformLocation(m_shader.id(), name.c_str());
            // m_uTextures.push_back(location);

            std::string name     = "uTextures[" + std::to_string(i) + "]";
            GLuint      location = glGetUniformLocation(m_shader.id(), name.c_str());
            m_uTextures.push_back(location);
        }
    }

    void UpdateUniforms()
    {
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glUniform1i(m_uNumTextures, m_textures.size());
    }

public:
    Mesh(std::vector<glimac::ShapeVertex>& vertices, const unsigned int& nbVertices, std::vector<Texture>& textures, const unsigned int& nbTextures)
    {
        InitVertexData(vertices, nbVertices);
        InitVao();
        InitTextures(textures, nbTextures);
        InitUniforms();
    }

    ~Mesh()
    {
        m_vao.UnBind();
        m_vbo.UnBind();
        m_vao.DeleteVao();
        m_vbo.DeleteVbo();
    }

    void update()
    {
    }

    void Render(glm::mat4& viewMatrix, p6::Context& ctx)
    {
        m_shader.use();
        UpdateMatrices(viewMatrix, ctx);
        UpdateUniforms();
        m_vao.Bind();

        for (size_t i = 0; i < m_textures.size(); ++i)
        {
            glUniform1i(m_uTextures[i], i);
            m_textures[i].Bind(i);
        }

        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

        for (size_t i = 0; i < m_textures.size(); ++i)
        {
            m_textures[i].UnBind(i);
        }
        m_vao.UnBind();
    }
};