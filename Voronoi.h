#pragma once

#include "Math.h"

#include "Triangle.h"

#include <vector>
#include <array>
#include <algorithm>

#include <SFML/System/Vector2.hpp>

namespace VoronoiMap {

    struct TriangleMap
    {
        std::vector<Triangle> myTriangles;
    };

    struct VoronoiCell
    {
        std::vector<sf::Vector2f> myVertex;
        std::vector<Edge> myEdges;
        sf::Vector2f              myCenter;
    };

    struct VoronoiMap
    {
        std::vector<VoronoiCell> myPoints;
    };

} // namespace Moria::Services::VoronoiMap#pragma once
