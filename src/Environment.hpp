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

    // Floor
    Texture              m_floorDiff{p6::load_image_buffer("assets/textures/environment/floor_diff.jpg")};
    std::vector<Texture> m_floorTextures = {m_floorDiff};
    Mesh                 m_floorMesh;

    // Sky
    Texture              m_skyTex{p6::load_image_buffer("assets/textures/environment/mountain.jpg")};
    Texture              m_skyTex2{p6::load_image_buffer("assets/textures/environment/mountain_mirrored.jpg")};
    Texture              m_skyTex3{p6::load_image_buffer("assets/textures/environment/cloud.jpg")};
    std::vector<Texture> m_skyTextures  = {m_skyTex};
    std::vector<Texture> m_skyTextures2 = {m_skyTex2};
    std::vector<Texture> m_skyTextures3 = {m_skyTex3};
    Mesh                 m_skyMesh;
    // Walls
    Mesh m_wall1Mesh;
    Mesh m_wall2Mesh;
    Mesh m_wall3Mesh;
    Mesh m_wall4Mesh;

    const float        m_width         = 1200.f;
    const unsigned int m_widthSegments = 200;

    // Arche
    std::vector<glimac::ShapeVertex> m_archeVertices;

    Texture              m_textureD{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Diffuse.png")};
    Texture              m_textureH{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Height.png"), 1};
    Texture              m_textureS{p6::load_image_buffer("assets/textures/environment/ArchSmall_Moss1-Specular.png"), 2};
    std::vector<Texture> m_archeTextures = {m_textureD, m_textureH, m_textureS};
    Mesh                 m_archeMesh;

    // Mountains
    std::vector<glimac::ShapeVertex> m_mountainVertices;
    std::vector<Texture>             m_mountainTextures = {m_floorDiff};
    Mesh                             m_mountainMesh;
    Mesh                             m_mountainMesh2;

    // Trees
    std::vector<glimac::ShapeVertex> m_treeVertices;
    Texture                          m_treeTexture{p6::load_image_buffer("assets/textures/environment/tree.jpg")};
    std::vector<Texture>             m_treeTextures = {m_treeTexture};
    Mesh                             m_treeMesh1;
    Mesh                             m_treeMesh2;

    // Floating Island
    std::vector<glimac::ShapeVertex> m_islandVertices;
    Texture                          m_islandTexture{p6::load_image_buffer("assets/textures/environment/floor_diff.jpg")};
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

    void RenderObjects(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);

    void RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);

    void DeleteTextures();
    void ShadowRender();
};