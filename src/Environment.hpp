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

class Environment {
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
    // Walls
    Mesh m_wall1Mesh;
    Mesh m_wall2Mesh;
    Mesh m_wall3Mesh;
    Mesh m_wall4Mesh;

    const float        m_width         = 200.f;
    const unsigned int m_widthSegments = 100;

    // Arche
    std::vector<glimac::ShapeVertex> m_archeVertices;

    Texture              m_textureD{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Diffuse.png")};
    Texture              m_textureH{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Height.png"), 1};
    Texture              m_textureN{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Normal.png"), 3};
    Texture              m_textureS{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Specular.png"), 2};
    std::vector<Texture> m_archeTextures = {m_textureD, m_textureH, m_textureN, m_textureS};
    Mesh                 m_archeMesh;

public:
    Environment() = default;

    void InitBorders();

    void InitArche();
    void InitMeshes();

    void RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx);

    void RenderArche(glm::mat4& viewMatrix, p6::Context& ctx);

    void RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx);
};