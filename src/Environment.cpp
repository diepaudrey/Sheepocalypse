#include "Environment.hpp"

void Environment::InitBorders()
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

void Environment::InitArche()
{
    m_archeVertices = LoadOBJ("./assets/models/SmallArch_Obj.obj");

    Mesh arche(m_archeVertices, m_archeTextures, glm::vec3(0.f, -25.f, 25.f), glm::vec3(0.f), glm::vec3(1.f));
    m_archeMesh = Mesh(arche);
}

void Environment::InitMeshes()
{
    InitBorders();
    InitArche();

    glEnable(GL_DEPTH_TEST);
}

void Environment::RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx)
{
    m_floorMesh.Render(viewMatrix, ctx);
    m_skyMesh.Render(viewMatrix, ctx);
    m_wall1Mesh.Render(viewMatrix, ctx);
    m_wall2Mesh.Render(viewMatrix, ctx);
    m_wall3Mesh.Render(viewMatrix, ctx);
    m_wall4Mesh.Render(viewMatrix, ctx);
}

void Environment::RenderArche(glm::mat4& viewMatrix, p6::Context& ctx)
{
    m_archeMesh.Render(viewMatrix, ctx);
}

void Environment::RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx)
{
    RenderBorders(viewMatrix, ctx);
    RenderArche(viewMatrix, ctx);
}

void Environment::DeleteTextures()
{
    m_textureS.DeleteTexture();
    m_textureD.DeleteTexture();
    m_textureH.DeleteTexture();
    m_floorTex.DeleteTexture();
    m_skyTex.DeleteTexture();
}