#include "GeometryUtils.h"
#include <vector>

// ------------------------------------------------------------
// Basic cross product (orientation test)
// ------------------------------------------------------------
static float Cross(const sf::Vector2f& a,
    const sf::Vector2f& b,
    const sf::Vector2f& c)
{
    return (b.x - a.x) * (c.y - a.y) -
        (b.y - a.y) * (c.x - a.x);
}

// ------------------------------------------------------------
// Signed polygon area (used to detect winding order)
// > 0  → Counter-clockwise
// < 0  → Clockwise
// ------------------------------------------------------------
static float SignedArea(const std::vector<sf::Vector2f>& poly)
{
    float area = 0.f;

    for (size_t i = 0; i < poly.size(); ++i)
    {
        const auto& a = poly[i];
        const auto& b = poly[(i + 1) % poly.size()];

        area += (a.x * b.y - b.x * a.y);
    }

    return area * 0.5f;
}

// ------------------------------------------------------------
// Check if point p is inside triangle (a, b, c)
// ------------------------------------------------------------
static bool PointInTriangle(const sf::Vector2f& p,
    const sf::Vector2f& a,
    const sf::Vector2f& b,
    const sf::Vector2f& c)
{
    float c1 = Cross(p, a, b);
    float c2 = Cross(p, b, c);
    float c3 = Cross(p, c, a);

    bool hasNeg = (c1 < 0) || (c2 < 0) || (c3 < 0);
    bool hasPos = (c1 > 0) || (c2 > 0) || (c3 > 0);

    return !(hasNeg && hasPos);
}

// ------------------------------------------------------------
// Ear clipping triangulation
// ------------------------------------------------------------
std::vector<std::vector<sf::Vector2f>>
ConvertConcaveToConvexPieces(const std::vector<sf::Vector2f>& polygon)
{
    std::vector<std::vector<sf::Vector2f>> result;

    if (polygon.size() < 3)
        return result;

    std::vector<sf::Vector2f> vertices = polygon;

    // Detect winding
    bool isCCW = SignedArea(vertices) > 0.f;

    while (vertices.size() >= 3)
    {
        bool earFound = false;

        for (size_t i = 0; i < vertices.size(); ++i)
        {
            sf::Vector2f prev =
                vertices[(i + vertices.size() - 1) % vertices.size()];
            sf::Vector2f curr = vertices[i];
            sf::Vector2f next =
                vertices[(i + 1) % vertices.size()];

            float cross = Cross(prev, curr, next);

            // Determine convexity based on winding
            bool isConvex = isCCW ? (cross > 0) : (cross < 0);

            if (!isConvex)
                continue;

            // Check if any other vertex lies inside the triangle
            bool containsPoint = false;

            for (size_t j = 0; j < vertices.size(); ++j)
            {
                if (j == i ||
                    j == (i + 1) % vertices.size() ||
                    j == (i + vertices.size() - 1) % vertices.size())
                    continue;

                if (PointInTriangle(vertices[j], prev, curr, next))
                {
                    containsPoint = true;
                    break;
                }
            }

            if (containsPoint)
                continue;

            // Found an ear → clip it
            result.push_back({ prev, curr, next });

            vertices.erase(vertices.begin() + i);
            earFound = true;
            break;
        }

        if (!earFound)
        {
            // Degenerate or malformed polygon safety exit
            break;
        }
    }

    return result;
}