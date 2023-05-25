#include "Environment.hpp"
#include "glimac/sphere_vertices.hpp"

void Environment::InitBorders(LightParams& lightP)
{
    m_planVertices = glimac::plan_vertices(m_width, m_width, m_widthSegments, m_widthSegments);

    m_wall1Mesh(m_planVertices, m_skyTextures2, glm::vec3(m_width / 2 - 10, 0.f, 0.f), glm::vec3(glm::radians(90.f), 0.f, glm::radians(90.f)), glm::vec3(1.f), lightP);

    m_wall2Mesh(m_planVertices, m_skyTextures2, glm::vec3(-m_width / 2 + 10, 0.f, 0.f), glm::vec3(glm::radians(90.f), 0.f, glm::radians(-90.f)), glm::vec3(1.f), lightP);

    m_wall3Mesh(m_planVertices, m_skyTextures, glm::vec3(0.f, 0.f, -m_width / 2 + 10), glm::vec3(glm::radians(90.f), 0.f, 0.f), glm::vec3(1.f), lightP);

    m_wall4Mesh(m_planVertices, m_skyTextures, glm::vec3(0.f, 0.f, m_width / 2 - 10), glm::vec3(glm::radians(-90.f), glm::radians(-180.f), 0.f), glm::vec3(1.f), lightP);
}

void Environment::InitMountain(LightParams& lightP)
{
    m_mountainVertices = LoadOBJ("./assets/models/Mountain.obj");

    m_mountainMesh(m_mountainVertices, m_mountainTextures, glm::vec3(-300.f, -595.f, 300.f), glm::vec3(0.f), glm::vec3(120.f), lightP);
    m_mountainMesh2(m_mountainVertices, m_mountainTextures, glm::vec3(300.f, -595.f, -300.f), glm::vec3(0.f), glm::vec3(120.f), lightP);
}

void Environment::InitTrees(LightParams& lightP)
{
    m_treeVertices = LoadOBJ("./assets/models/Mountain.obj");
    m_treeMesh1(m_treeVertices, m_treeTextures, glm::vec3(-300.f, -595.f, -300.f), glm::vec3(0.f), glm::vec3(120.f), lightP);
    m_treeMesh2(m_treeVertices, m_treeTextures, glm::vec3(300.f, -595.f, 300.f), glm::vec3(0.f), glm::vec3(120.f), lightP);
}

void Environment::InitIsland(LightParams& lightP)
{
    m_islandVertices = LoadOBJ("./assets/models/FloatingIsland2.obj");
    m_islandMesh1(m_islandVertices, m_islandTextures, glm::vec3(-150.f, -30.f, -200.f), glm::vec3(0.f), glm::vec3(70.f), lightP);
    m_islandMesh2(m_islandVertices, m_islandTextures, glm::vec3(180.f, 160.f, 100.f), glm::vec3(0.f), glm::vec3(50.f), lightP);
    m_islandMesh3(m_islandVertices, m_islandTextures, glm::vec3(-30.f, -150.f, 400.f), glm::vec3(0.f), glm::vec3(30.f), lightP);
}

void Environment::InitMeshes(LightParams& lightP)
{
    InitBorders(lightP);
    InitMountain(lightP);
    InitTrees(lightP);
    InitIsland(lightP);
}

void Environment::RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    m_wall1Mesh.Render(viewMatrix, ctx, lightP);
    m_wall2Mesh.Render(viewMatrix, ctx, lightP);
    m_wall3Mesh.Render(viewMatrix, ctx, lightP);
    m_wall4Mesh.Render(viewMatrix, ctx, lightP);
}

void Environment::RenderElements(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    m_mountainMesh.Render(viewMatrix, ctx, lightP);
    m_mountainMesh2.Render(viewMatrix, ctx, lightP);
    m_treeMesh1.Render(viewMatrix, ctx, lightP);
    m_treeMesh2.Render(viewMatrix, ctx, lightP);
    m_islandMesh1.Render(viewMatrix, ctx, lightP);
    m_islandMesh2.Render(viewMatrix, ctx, lightP);
    m_islandMesh3.Render(viewMatrix, ctx, lightP);
}

void Environment::RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    RenderBorders(viewMatrix, ctx, lightP);
    RenderElements(viewMatrix, ctx, lightP);
}

// void Environment::ShadowRender()
// {
//     m_archeMesh.BasicRender();
// }

void Environment::DeleteTextures()
{
    // m_textureS.DeleteTexture();
    // m_textureD.DeleteTexture();
    // m_textureH.DeleteTexture();
    m_floorDiff.DeleteTexture();
    m_skyTex.DeleteTexture();
}
