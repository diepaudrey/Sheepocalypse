#pragma once
#include <vector>
#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
std::vector<ShapeVertex> plan_vertices(float width, float depth, size_t widthSegments, size_t depthSegments);

} // namespace glimac