#include "Player.hpp"

void Player::InitPlayer(glm::vec3 position, LightParams& lightP)
{
    m_position       = position;
    m_playerVertices = LoadOBJ("./assets/models/Night_Fury.obj");
    m_playerMesh(m_playerVertices, m_playerTextures, m_position, glm::vec3(0.f), glm::vec3(2.f), lightP);
}

// void Player::UpdatePosition(glm::vec3 position)
// {
//     m_position = position;
// }

void Player::RenderPlayer(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP, glm::vec3& position, glm::vec3& rotation)
{
    m_position = position;
    m_rotation = rotation;
    m_playerMesh.RenderMoving(viewMatrix, ctx, lightP, m_position, rotation);
}