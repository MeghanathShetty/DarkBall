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

// Check if point p is inside triangle (a, b, c)
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

// Ear clipping triangulation 
// Convert polygon with high vertices to smaller triangles with 3 vertices
// Later when these smaller triangles are merged toether,
// they become the shape of the orignal polygon
std::vector<std::vector<sf::Vector2f>>
EarClipTriangulate(const std::vector<sf::Vector2f>& polygon)
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

std::vector<sf::Vector2f>
// Used to generate smooth curves from control points (e.g. for terrain)
// For each segment (P1 → P2), generate smooth intermediate
// points between them, P1 → s1 → s2 → ... → P2
GenerateCatmullRom(const std::vector<sf::Vector2f>& ctrl,
    int samplesPerSeg)
{
    std::vector<sf::Vector2f> out;

    if (ctrl.size() < 2)
        return out;

    auto CR = [](const sf::Vector2f& p0,
        const sf::Vector2f& p1,
        const sf::Vector2f& p2,
        const sf::Vector2f& p3,
        float t)
        {
            float t2 = t * t;
            float t3 = t2 * t;

            sf::Vector2f result;

            result.x = 0.5f * (
                (2.f * p1.x) +
                (-p0.x + p2.x) * t +
                (2.f * p0.x - 5.f * p1.x + 4.f * p2.x - p3.x) * t2 +
                (-p0.x + 3.f * p1.x - 3.f * p2.x + p3.x) * t3
                );

            result.y = 0.5f * (
                (2.f * p1.y) +
                (-p0.y + p2.y) * t +
                (2.f * p0.y - 5.f * p1.y + 4.f * p2.y - p3.y) * t2 +
                (-p0.y + 3.f * p1.y - 3.f * p2.y + p3.y) * t3
                );

            return result;
        };

    std::vector<sf::Vector2f> pts = ctrl;

    // duplicate endpoints for boundary
    pts.insert(pts.begin(), pts.front());
    pts.push_back(pts.back());

    for (size_t i = 0; i + 3 < pts.size(); ++i)
    {
        for (int s = 0; s < samplesPerSeg; ++s)
        {
            float t = s / static_cast<float>(samplesPerSeg);

            out.push_back(
                CR(pts[i], pts[i + 1],
                    pts[i + 2], pts[i + 3], t)
            );
        }
    }

    out.push_back(ctrl.back());

    return out;
}