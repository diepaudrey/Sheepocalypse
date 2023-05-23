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

    // Floor
    Texture m_floorDiff{p6::load_image_buffer("assets/textures/environment/floor_diff.jpg")};
    // Texture m_floorDisp{p6::load_image_buffer("assets/textures/environment/floor_disp.png"), 1};
    // Texture              m_floorRough{p6::load_image_buffer("assets/textures/environment/floor_rough.jpg"), 2};
    std::vector<Texture> m_floorTextures = {m_floorDiff};
    Mesh                 m_floorMesh;

    // Sky
    Texture              m_skyTex{p6::load_image_buffer("assets/textures/environment/test.jpg")};
    std::vector<Texture> m_skyTextures = {m_skyTex};
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

    void InitBorders(LightParams& lightP);
    void InitArche(LightParams& lightP);
    void InitMountain(LightParams& lightP);
    void InitTrees(LightParams& lightP);

public:
    Environment() = default;
    void InitMeshes(LightParams& lightP);
    void RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);

    void RenderArche(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);

    void RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP);

    void DeleteTextures();
    void ShadowRender();
};