#include "Game.hpp"

Game::Game(p6::Context& ctx, BoidsParameters& boidParam)
{
    InitBoids(ctx, boidParam);
    InitCamera();
    mouseHandler(ctx);
    InitImGui(boidParam);
    InitLight();
    InitEnvironment();

    //  std::cout << "Game initialized" << std::endl;
}

void Game::mouseHandler(p6::Context& ctx)
{
    /*La caméra sans souris*/

    // glfwSetInputMode(ctx.underlying_glfw_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ctx.mouse_moved = [&](p6::MouseMove data) {
    //     (camera).rotateLeft(data.delta.x * rotationStrength);
    //     (camera).rotateUp(-data.delta.y * rotationStrength);
    // };

    /*La caméra avec cliquer + glisser*/
    ctx.mouse_dragged = [&](p6::MouseDrag data) {
        (m_cam).rotateLeft(data.delta.x * rotationStrength);
        (m_cam).rotateUp(-data.delta.y * rotationStrength);
    };
}

void Game::keyboardHandler(p6::Context& ctx)
{
    if (ctx.key_is_pressed(GLFW_KEY_W))
    {
        m_cam.moveFront(ctx.delta_time() * movementStrength);
    };
    if (ctx.key_is_pressed(GLFW_KEY_S))
    {
        m_cam.moveFront(-ctx.delta_time() * movementStrength);
    }
    if (ctx.key_is_pressed(GLFW_KEY_A))
    {
        m_cam.moveLeft(ctx.delta_time() * movementStrength);
    }
    if (ctx.key_is_pressed(GLFW_KEY_D))
    {
        m_cam.moveLeft(-ctx.delta_time() * movementStrength);
    }
}

void Game::InitBoids(p6::Context& ctx, BoidsParameters& boidParam)
{
    std::vector<Boid> vecBoids;
    m_boids = Boids(vecBoids, m_nbBoids, boidParam);
    m_boids.fillBoids(ctx);
}

void Game::InitCamera()
{
    m_cam = glimac::FreeflyCamera();
}

void Game::InitImGui(BoidsParameters& boidParam)
{
    boidParam.protectedRadius    = protectedRadius;
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
    lightP.light          = glm::vec3(0.f, 0.f, 0.f);
    lightP.lightIntensity = glm::vec3(5000.f);
    lightP.Ka             = glm::vec3(0.05, 0.05, 0.05);
    lightP.Kd             = glm::vec3(1.0, 1.0, 1.0);
    lightP.Ks             = glm::vec3(1.0, 1.0, 1.0);
    lightP.shininess      = 0.5f;
    std::cout << "ligh p : " << lightP.light.x << " " << lightP.light.y << " " << lightP.light.z << std::endl;
    lightGame.initLight(lightP);
}

void Game::Render(p6::Context& ctx, BoidsParameters& boidParam)
{
    keyboardHandler(ctx);
    viewMatrix  = m_cam.getViewMatrix();
    verticesPtr = &verticesLow;
    if (boidParam.lodMid)
    {
        verticesPtr = &verticesMedium;
    }
    else if (boidParam.lodHigh)
    {
        verticesPtr = &verticesHigh;
    }
    boidParam.updateBoidsParam();
    m_boids.updateBoids(ctx, boidParam);
    m_boids.drawBoids(ctx, viewMatrix, *verticesPtr, lightP);
    m_environment.RenderMeshes(viewMatrix, ctx, lightP);
}
