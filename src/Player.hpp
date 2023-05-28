#pragma once

#include "Mesh.hpp"
#include "OBJLoader.hpp"

class Player {
public:
    std::vector<glimac::ShapeVertex> m_playerVertices;

    Texture              m_playerTex{p6::load_image_buffer("assets/textures/DrakeNatural.jpg")};
    std::vector<Texture> m_playerTextures = {m_playerTex};
    Mesh                 m_playerMesh;

    glm::vec3 m_position{};
    glm::vec3 m_rotation{};

public:
    Player()  = default;
    ~Player() = default;

    void InitPlayer(glm::vec3 position, LightParams& lightP);
    void RenderPlayer(glm::mat4& viewMatrix, p6::Context& ctx, LightParams& lightP, glm::vec3& position, glm::vec3& rotation);
};