#include "Game.hpp"

Game::Game(p6::Context& ctx, BoidsParameters& boidParam)
{
    InitBoids();
    InitCamera();
    MouseHandler(ctx);
    InitImGui(boidParam);
    InitLight();
    InitEnvironment();
    InitPlayer();
    // m_DepthMap(glGetUniformLocation(m_shadowShader.id(), "depthMVP"));
    // m_DepthText(glGetUniformLocation(m_shader.id(), "uDepthTexture"));
    // m_shadowMap.InitWindow(1024, 1024);
}

void Game::MouseHandler(p6::Context& ctx)
{
    glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    ctx.mouse_moved = [&](p6::MouseMove data) {
        (m_cam).RotateLeft(data.delta.x * rotationStrength);
        (m_cam).RotateUp(-data.delta.y * rotationStrength);
    };
}

void Game::KeyboardHandler(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        if (!m_cam.FixCamLimit(m_limit))
        {
            m_cam.MoveFront(ctx.delta_time() * movementStrength);
        }
    };
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        if (!m_cam.FixCamLimit(m_limit))
        {
            m_cam.MoveFront(-ctx.delta_time() * movementStrength);
        }
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        if (!m_cam.FixCamLimit(m_limit))
        {
            m_cam.MoveLeft(ctx.delta_time() * movementStrength);
        }
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        if (!m_cam.FixCamLimit(m_limit))
        {
            m_cam.MoveLeft(-ctx.delta_time() * movementStrength);
        }
    }
    ctx.key_pressed = [&](const p6::Key& data) {
        if (data.physical == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(ctx.underlying_glfw_window(), GLFW_TRUE);
        }
        else if (data.physical == GLFW_KEY_SPACE)
        {
            m_cam.TogglePause();
            if (m_cam.IsPaused())
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
    m_boids.FillBoids();
}

void Game::InitPlayer()
{
    glm::vec3 playerPosition = m_cam.GetPosition();
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
    lightGame.InitializeLight(lightP);

    lightP2.light          = m_cam.GetPosition();
    lightP2.lightIntensity = glm::vec3(2.f);
    lightP2.Ka             = glm::vec3(0.05, 0.05, 0.05);
    lightP2.Kd             = glm::vec3(1.0, 1.0, 1.0);
    lightP2.Ks             = glm::vec3(1.0, 1.0, 1.0);
    lightP2.shininess      = 0.4f;
    lightPlayer.InitializeLight(lightP2);
}

void Game::UpdateLightPlayer()
{
    lightP2.light = m_cam.GetPosition();
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

    KeyboardHandler(ctx);
    viewMatrix = m_cam.GetViewMatrix();
    ChangeLOD(boidParam);
    m_boids.UpdateBoids(ctx, boidParam);
    m_boids.DrawBoids(ctx, viewMatrix, *verticesPtr, lightP);

    // glm::mat4 LightView    = glm::lookAt(lightP.light, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
    // glm::mat4 OrthoProjMat = glm::ortho(-80.f, 80.f, -80.f, 80.f, 0.1f, 75.f);
    // glm::mat4 WVP          = OrthoProjMat * LightView;
    // glUniformMatrix4fv(glGetUniformLocation(m_shader.id(), "uLightProjection"), 1, GL_FALSE, glm::value_ptr(WVP));
    // m_shadowMap.BindForReading(GL_TEXTURE10);
    // glUniform1i(glGetUniformLocation(m_shader.id(), "uDepthTexture"), 10);
    m_environment.RenderMeshes(viewMatrix, ctx, lightP);

    // m_shadowMap.UnBind(GL_TEXTURE10);

    UpdateLightPlayer();
    m_player.m_position = m_cam.GetPosition() + glm::vec3(5.f, -10.f, 5.f);
    m_player.m_rotation = m_cam.GetUpVector();
    glm::mat4 vmat      = glm::lookAt(m_cam.GetPosition(), m_player.m_position, glm::vec3(0, 1, 0));
    m_player.RenderPlayer(vmat, ctx, lightP2, m_player.m_position, m_player.m_rotation);
}

// void Game::RenderShadow()
// {
//     glm::mat4 LightView    = glm::lookAt(lightP.light, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
//     glm::mat4 OrthoProjMat = glm::ortho(-80.f, 80.f, -80.f, 80.f, 0.1f, 75.f);
//     glm::mat4 WVP          = OrthoProjMat * LightView;

//     m_shadowShader.use();
//     m_shadowMap.setShadow(WVP);
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
