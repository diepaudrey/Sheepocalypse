#include "Game.hpp"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"

Game::Game(p6::Context& ctx, BoidsParameters& boidParam)
    : m_DepthMap(glGetUniformLocation(m_shadowShader.id(), "depthMVP")), m_DepthText(glGetUniformLocation(m_shader.id(), "uDepthTexture"))
{
    InitBoids();
    InitCamera();
    mouseHandler(ctx);
    InitImGui(boidParam);
    InitLight();
    InitEnvironment();
    InitPlayer();

    // m_shadowMap.InitWindow(1024, 1024);
    // glEnable(GL_DEPTH_TEST);
}

void Game::mouseHandler(p6::Context& ctx)
{
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ctx.mouse_moved = [&](p6::MouseMove data) {
        (m_cam).rotateLeft(data.delta.x * rotationStrength);
        (m_cam).rotateUp(-data.delta.y * rotationStrength);
    };
}

void Game::keyboardHandler(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        if (m_cam.fixCamLimit(m_limit) == false)
        {
            m_cam.moveFront(ctx.delta_time() * movementStrength);
        }
    };
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        if (m_cam.fixCamLimit(m_limit) == false)
        {
            m_cam.moveFront(-ctx.delta_time() * movementStrength);
        }
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        if (m_cam.fixCamLimit(m_limit) == false)
        {
            m_cam.moveLeft(ctx.delta_time() * movementStrength);
        }
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        if (m_cam.fixCamLimit(m_limit) == false)
        {
            m_cam.moveLeft(-ctx.delta_time() * movementStrength);
        }
    }
    ctx.key_pressed = [&](const p6::Key& data) {
        if (data.physical == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(ctx.underlying_glfw_window(), GLFW_TRUE);
        }
        else if (data.physical == GLFW_KEY_SPACE)
        {
            m_cam.togglePause();
            if (m_cam.isPaused())
            {
                glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    };
}

void Game::InitBoids()
{
    std::vector<Boid> vecBoids;
    m_boids = Boids(vecBoids, m_nbBoids);
    m_boids.fillBoids();
}

void Game::InitPlayer()
{
    glm::vec3 playerPosition = m_cam.getPosition();
    playerPosition.z += 10.f;
    m_player.InitPlayer(playerPosition, lightP2);
}

void Game::InitCamera()
{
    m_cam = glimac::FreeflyCamera();
}

void Game::InitImGui(BoidsParameters& boidParam) const
{
    boidParam.protectedRadius    = protectedRadius;
    boidParam.visualRange        = visualRange;
    boidParam.alignmentStrength  = alignmentStrength;
    boidParam.cohesionStrength   = cohesionStrength;
    boidParam.separationStrength = separationStrength;
    boidParam.maxSpeed           = maxSpeed;
    boidParam.lodLow             = lodLow;
    boidParam.lodMid             = lodMid;
    boidParam.lodHigh            = lodHigh;
}

void Game::InitEnvironment()
{
    m_environment.InitMeshes(lightP);
}

void Game::InitLight()
{
    lightP.light          = glm::vec3(0.f, 600.f, 0.f);
    lightP.lightIntensity = glm::vec3(1.0f);
    lightP.Ka             = glm::vec3(0.05, 0.05, 0.05);
    lightP.Kd             = glm::vec3(1.0, 1.0, 1.0);
    lightP.Ks             = glm::vec3(1.0, 1.0, 1.0);
    lightP.shininess      = 0.2f;
    lightGame.initLight(lightP);

    lightP2.light          = m_cam.getPosition();
    lightP2.lightIntensity = glm::vec3(3.f);
    lightP2.Ka             = glm::vec3(0.05, 0.05, 0.05);
    lightP2.Kd             = glm::vec3(1.0, 1.0, 1.0);
    lightP2.Ks             = glm::vec3(1.0, 1.0, 1.0);
    lightP2.shininess      = 0.4f;
    lightPlayer.initLight(lightP2);
}

void Game::UpdateLightPlayer()
{
    lightP2.light = m_cam.getPosition();
}

void Game::ChangeLOD(BoidsParameters& boidParam)
{
    verticesPtr = &verticesLow;
    if (boidParam.lodMid)
    {
        verticesPtr = &verticesMedium;
    }
    else if (boidParam.lodHigh)
    {
        verticesPtr = &verticesHigh;
    }
}

void Game::Render(p6::Context& ctx, BoidsParameters& boidParam)
{
    glViewport(0, 0, ctx.main_canvas_width(), ctx.main_canvas_height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    keyboardHandler(ctx);
    viewMatrix = m_cam.getViewMatrix();
    ChangeLOD(boidParam);
    boidParam.updateBoidsParam();

    // glm::mat4 LightView    = glm::lookAt(lightP.light, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    // glm::mat4 OrthoProjMat = glm::ortho(-80.f, 80.f, -80.f, 80.f, 0.1f, 75.f);
    // glm::mat4 WVP          = OrthoProjMat * LightView;
    // glUniformMatrix4fv(glGetUniformLocation(m_shader.id(), "uLightProjection"), 1, GL_FALSE, glm::value_ptr(WVP));
    // m_shadowMap.BindForReading(GL_TEXTURE10);
    // glUniform1i(glGetUniformLocation(m_shader.id(), "uDepthTexture"), 10);
    m_environment.RenderMeshes(viewMatrix, ctx, lightP);
    m_boids.updateBoids(ctx, boidParam);
    m_boids.drawBoids(ctx, viewMatrix, *verticesPtr, lightP);

    // m_shadowMap.UnBind(GL_TEXTURE10);

    UpdateLightPlayer();
    m_player.m_position = m_cam.getPosition() + glm::vec3(5.f, -10.f, 5.f);
    m_player.m_rotation = m_cam.getUpVector();
    glm::mat4 vmat      = glm::lookAt(m_cam.getPosition(), m_player.m_position, glm::vec3(0, 1, 0));
    m_player.RenderPlayer(vmat, ctx, lightP2, m_player.m_position, m_player.m_rotation);

    // std::cout << m_player.m_position.x << " " << m_player.m_position.y << " " << m_player.m_position.z << std::endl;
}

// void Game::RenderShadow()
// {
//     glm::mat4 LightView    = glm::lookAt(lightP.light, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
//     glm::mat4 OrthoProjMat = glm::ortho(-80.f, 80.f, -80.f, 80.f, 0.1f, 75.f);
//     glm::mat4 WVP          = OrthoProjMat * LightView;

//     m_shadowShader.use();
//     m_shadowMap.setShadow(WVP);
//     glEnable(GL_DEPTH_TEST);
//     m_shadowMap.BindForWriting();
//     glClear(GL_DEPTH_BUFFER_BIT);
//     m_environment.ShadowRender();
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

void Game::RenderFinal(p6::Context& ctx, BoidsParameters& boidParam)
{
    glEnable(GL_DEPTH_TEST);
    // RenderShadow();
    Render(ctx, boidParam);
    glDisable(GL_DEPTH_TEST);
}
