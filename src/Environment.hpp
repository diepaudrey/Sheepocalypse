#pragma once

#include <iostream>
#include <vector>
#include "Mesh.hpp"
#include "OBJLoader.hpp"
// #include "glimac/common.hpp"
// a trier
#include "Shadow.hpp"
#include "glimac/plan_vertices.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"

class Environment {
private:
    std::vector<glimac::ShapeVertex> m_planVertices;
    std::vector<glimac::ShapeVertex> sphere_vertices(float radius, size_t discretization_latitude, size_t discretization_longitude);

    // Sky
    Mesh                 m_skyMesh;
    Texture              m_skyText{p6::load_image_buffer("assets/textures/environment/sky.jpg")};
    std::vector<Texture> m_skyTextures = {m_skyText};
    // Walls
    Mesh                 m_wall1Mesh;
    Mesh                 m_wall2Mesh;
    Mesh                 m_wall3Mesh;
    Mesh                 m_wall4Mesh;
    Texture              m_wallTex{p6::load_image_buffer("assets/textures/environment/mountain.jpg")};
    Texture              m_wallTex2{p6::load_image_buffer("assets/textures/environment/mountain2.jpg")};
    std::vector<Texture> m_wallTextures  = {m_wallTex};
    std::vector<Texture> m_wallTextures2 = {m_wallTex2};

    const float        m_width         = 1200.f;
    const unsigned int m_widthSegments = 200;

    // Mountains
    std::vector<glimac::ShapeVertex> m_mountainVertices;
    Mesh                             m_mountainMesh;
    Mesh                             m_mountainMesh2;
    Texture                          m_mountainText{p6::load_image_buffer("assets/textures/environment/floor_diff.jpg")};
    std::vector<Texture>             m_mountainTextures = {m_mountainText};

    // Trees
    std::vector<glimac::ShapeVertex> m_treeVertices;
    Texture                          m_treeTexture{p6::load_image_buffer("assets/textures/environment/tree.jpg")};
    std::vector<Texture>             m_treeTextures = {m_treeTexture};
    Mesh                             m_treeMesh1;
    Mesh                             m_treeMesh2;

    // Floating Island
    std::vector<glimac::ShapeVertex> m_islandVertices;
    Texture                          m_islandTexture{p6::load_image_buffer("assets/textures/environment/island_diff.jpg")};
    std::vector<Texture>             m_islandTextures = {m_islandTexture};
    Mesh                             m_islandMesh1;
    Mesh                             m_islandMesh2;
    Mesh                             m_islandMesh3;

    // Collision
    std::vector<glm::vec3> m_verticesCollision;

    void InitBorders(LightParams& lightP);
    void InitArche(LightParams& lightP);
    void InitMountain(LightParams& lightP);
    void InitTrees(LightParams& lightP);
    void InitIsland(LightParams& lightP);

public:
    Environment() = default;
    void InitMeshes(LightParams& lightP);
    void fillVerticesCollision();
    void RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);
    void RenderElements(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);
    void RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);
    void ShadowRender();
};