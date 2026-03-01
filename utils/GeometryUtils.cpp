#include "GeometryUtils.h"

static float Cross(const sf::Vector2f& a,
    const sf::Vector2f& b,
    const sf::Vector2f& c)
{
    return (b.x - a.x) * (c.y - a.y) -
        (b.y - a.y) * (c.x - a.x);
}

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

std::vector<std::vector<sf::Vector2f>>
ConvertConcaveToConvexPieces(const std::vector<sf::Vector2f>& polygon)
{
    std::vector<std::vector<sf::Vector2f>> result;

    if (polygon.size() < 3)
        return result;

    std::vector<sf::Vector2f> vertices = polygon;

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

            // Check if vertex is convex (assumes clockwise winding)
            if (Cross(prev, curr, next) >= 0)
                continue;

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

            // Found an ear → create triangle
            result.push_back({ prev, curr, next });

            vertices.erase(vertices.begin() + i);
            earFound = true;
            break;
        }

        if (!earFound)
            break; // safety exit
    }

    return result;
}