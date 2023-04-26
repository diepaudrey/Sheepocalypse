#pragma once

#include <iostream>
#include <vector>
#include "Mesh.hpp"
#include "OBJLoader.hpp"
// #include "glimac/common.hpp"
// a trier
#include "glimac/plan_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"

class RendererBorders {
private:
    std::vector<glimac::ShapeVertex> m_planVertices;
    // Floor
    Texture              floorTex{p6::load_image_buffer("assets/textures/Grass.jpg")};
    std::vector<Texture> m_floorTextures = {floorTex};
    Mesh                 m_floorMesh;

    // Sky
    Texture              skyTex{p6::load_image_buffer("assets/textures/Sky.jpg")};
    std::vector<Texture> m_skyTextures = {skyTex};
    Mesh                 m_skyMesh;

    Mesh m_wall1Mesh;
    Mesh m_wall2Mesh;
    Mesh m_wall3Mesh;
    Mesh m_wall4Mesh;

    const float        m_width         = 100.f;
    const unsigned int m_widthSegments = 32;

public:
    RendererBorders() = default;

    void InitBorders()
    {
        m_planVertices = glimac::plan_vertices(m_width, m_width, m_widthSegments, m_widthSegments);
        Mesh plan(m_planVertices, m_floorTextures, glm::vec3(0.f, -m_width / 2, 0.f), glm::vec3(0.f), glm::vec3(1.f));
        m_floorMesh = Mesh(plan);

        Mesh sky(m_planVertices, m_skyTextures, glm::vec3(0.f, m_width / 2, 0.f), glm::vec3(0.f), glm::vec3(1.f));
        m_skyMesh = Mesh(sky);

        Mesh wall1(m_planVertices, m_skyTextures, glm::vec3(m_width / 2, 0.f, 0.f), glm::vec3(0.f, 0.f, glm::radians(90.f)), glm::vec3(1.f));
        m_wall1Mesh = Mesh(wall1);

        Mesh wall2(m_planVertices, m_skyTextures, glm::vec3(-m_width / 2, 0.f, 0.f), glm::vec3(0.f, 0.f, glm::radians(90.f)), glm::vec3(1.f));
        m_wall2Mesh = Mesh(wall2);

        Mesh wall3(m_planVertices, m_skyTextures, glm::vec3(0.f, 0.f, -m_width / 2), glm::vec3(glm::radians(90.f), 0.f, 0.f), glm::vec3(1.f));
        m_wall3Mesh = Mesh(wall3);

        Mesh wall4(m_planVertices, m_skyTextures, glm::vec3(0.f, 0.f, m_width / 2), glm::vec3(glm::radians(90.f), 0.f, 0.f), glm::vec3(1.f));
        m_wall4Mesh = Mesh(wall4);
    }

    void RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx)
    {
        m_floorMesh.Render(viewMatrix, ctx);
        m_skyMesh.Render(viewMatrix, ctx);
        m_wall1Mesh.Render(viewMatrix, ctx);
        m_wall2Mesh.Render(viewMatrix, ctx);
        m_wall3Mesh.Render(viewMatrix, ctx);
        m_wall4Mesh.Render(viewMatrix, ctx);
    }
};