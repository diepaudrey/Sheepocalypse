#include "Game.hpp"
#include <iostream>

Game::Game(p6::Context& ctx)
{
    InitBoids(ctx);
    InitCamera();
    mouseHandler(ctx);
    // InitImGui();
    InitEnvironment();
    std::cout << "Game initialized" << std::endl;
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

void Game::InitBoids(p6::Context& ctx)
{
    std::vector<Boid> vecBoids;
    m_boids = Boids(vecBoids, m_nbBoids);
    m_boids.fillBoids(ctx);
}

void Game::InitCamera()
{
    m_cam = glimac::FreeflyCamera();
}

// void Game::InitImGui()
// {
//     IHM = sImGui(protectedRadius, separationStrength, alignmentStrength, cohesionStrength, maxSpeed, lodDragon);
// }

void Game::InitEnvironment()
{
    m_environment.InitMeshes();
}

void Game::Render(p6::Context& ctx)
{
    keyboardHandler(ctx);
    viewMatrix  = m_cam.getViewMatrix();
    verticesPtr = &verticesHigh;
    m_boids.setProtectedRadius(protectedRadius);
    m_boids.setAlignmentStrength(alignmentStrength);
    m_boids.setCohesionStrength(cohesionStrength);
    m_boids.setSeparationStrength(separationStrength);
    m_boids.setBoidsMaxSpeed(maxSpeed);
    m_boids.updateBoids(ctx);
    m_boids.drawBoids(ctx, viewMatrix, *verticesPtr);
    m_environment.RenderMeshes(viewMatrix, ctx);
}
