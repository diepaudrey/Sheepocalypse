#include "Environment.hpp"

void Environment::InitBorders(LightParams& lightP)
{
    m_planVertices = glimac::plan_vertices(m_width, m_width, m_widthSegments, m_widthSegments);

    m_floorMesh(m_planVertices, m_floorTextures, glm::vec3(0.f, -m_width / 2, 0.f), glm::vec3(0.f), glm::vec3(1.f), lightP);

    // m_skyMesh(m_planVertices, m_skyTextures, glm::vec3(0.f, m_width / 2, 0.f), glm::vec3(0.f, 0.f, glm::radians(180.f)), glm::vec3(1.f), lightP);

    m_wall1Mesh(m_planVertices, m_skyTextures, glm::vec3(m_width / 2, 0.f, 0.f), glm::vec3(0.f, 0.f, glm::radians(90.f)), glm::vec3(1.f), lightP);

    m_wall2Mesh(m_planVertices, m_skyTextures, glm::vec3(-m_width / 2, 0.f, 0.f), glm::vec3(0.f, 0.f, glm::radians(-90.f)), glm::vec3(1.f), lightP);

    m_wall3Mesh(m_planVertices, m_skyTextures, glm::vec3(0.f, 0.f, -m_width / 2), glm::vec3(glm::radians(90.f), 0.f, 0.f), glm::vec3(1.f), lightP);

    m_wall4Mesh(m_planVertices, m_skyTextures, glm::vec3(0.f, 0.f, m_width / 2), glm::vec3(glm::radians(-90.f), 0.f, 0.f), glm::vec3(1.f), lightP);
}

void Environment::InitArche(LightParams& lightP)
{
    m_archeVertices = LoadOBJ("./assets/models/SmallArch_Obj.obj");

    m_archeMesh(m_archeVertices, m_archeTextures, glm::vec3(40.f, -80.f, 25.f), glm::vec3(0.f), glm::vec3(0.5f), lightP);
}

void Environment::InitMeshes(LightParams& lightP)
{
    InitBorders(lightP);
    InitArche(lightP);

    // glEnable(GL_DEPTH_TEST);
}

void Environment::RenderBorders(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    m_floorMesh.Render(viewMatrix, ctx, lightP);
    // m_skyMesh.Render(viewMatrix, ctx, lightP);
    m_wall1Mesh.Render(viewMatrix, ctx, lightP);
    m_wall2Mesh.Render(viewMatrix, ctx, lightP);
    m_wall3Mesh.Render(viewMatrix, ctx, lightP);
    m_wall4Mesh.Render(viewMatrix, ctx, lightP);
}

void Environment::RenderArche(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    m_archeMesh.Render(viewMatrix, ctx, lightP);
    // m_archeMesh.ShadowMapPass(viewMatrix, lightP.light);
    // m_archeMesh.RenderShadow(lightP.light);
}

void Environment::RenderMeshes(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP)
{
    RenderBorders(viewMatrix, ctx, lightP);
    RenderArche(viewMatrix, ctx, lightP);
}

void Environment::ShadowRender()
{
    m_floorMesh.BasicRender();
    // m_wall1Mesh.BasicRender();
    // m_wall2Mesh.BasicRender();
    // m_wall3Mesh.BasicRender();
    // m_wall4Mesh.BasicRender();
    m_archeMesh.BasicRender();
}

void Environment::DeleteTextures()
{
    m_textureS.DeleteTexture();
    m_textureD.DeleteTexture();
    m_textureH.DeleteTexture();
    m_floorDiff.DeleteTexture();
    // m_floorDisp.DeleteTexture();
    // m_floorRough.DeleteTexture();
    m_skyTex.DeleteTexture();
}