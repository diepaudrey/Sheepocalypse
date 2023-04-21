#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include "sphere_vertices.hpp"

namespace glimac {

/*widthSegments représente le nombre de segments (c'est-à-dire le nombre de subdivisions) le long de l'axe horizontal du plan.
depthSegments représente le nombre de segments le long de l'axe vertical du plan.
depth représente la profondeur du plan, c'est-à-dire la distance entre sa surface supérieure et sa surface inférieure.*/

std::vector<ShapeVertex> plan_vertices(float width, float depth, size_t widthSegments, size_t depthSegments) // NOLINT(bugprone-easily-swappable-parameters, readability-inconsistent-declaration-parameter-name)
{
    const auto fWidthSegments = static_cast<float>(widthSegments);
    const auto fDepthSegments = static_cast<float>(depthSegments);

    const float dx = width / fWidthSegments;
    const float dz = depth / fDepthSegments;

    std::vector<ShapeVertex> data;

    // Construit l'ensemble des vertex
    for (size_t j = 0; j <= depthSegments; ++j)
    {
        for (size_t i = 0; i <= widthSegments; ++i)
        {
            const auto fi = static_cast<float>(i);
            const auto fj = static_cast<float>(j);

            ShapeVertex vertex{};

            vertex.texCoords.x = fi / fWidthSegments;
            vertex.texCoords.y = fj / fDepthSegments;

            vertex.position.x = fi * dx - width / 2.f;
            vertex.position.y = 0.f;
            vertex.position.z = fj * dz - depth / 2.f;

            vertex.normal = {0.f, 1.f, 0.f};

            data.push_back(vertex);
        }
    }

    std::vector<ShapeVertex> vertices{};
    // Construit les vertex finaux en regroupant les données en triangles :
    // Pour chaque rectangle formé par 4 vertices, les deux triangles formant une face sont de la forme :
    // (i, i + widthSegments + 1, i + 1), (i, i + widthSegments, i + widthSegments + 1)
    // avec i sur le rectangle
    for (size_t j = 0; j < depthSegments; ++j)
    {
        const size_t offset = j * (widthSegments + 1);
        for (size_t i = 0; i < widthSegments; ++i)
        {
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + i + widthSegments + 1]);
            vertices.push_back(data[offset + i + 1]);
            vertices.push_back(data[offset + i]);
            vertices.push_back(data[offset + i + widthSegments]);
            vertices.push_back(data[offset + i + widthSegments + 1]);
        }
    }

    return vertices;
}

} // namespace glimac
