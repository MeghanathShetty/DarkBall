#include "Level.h"
#include "../utils/GeometryUtils.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void Level::load(const std::string& filename, b2World& world)
{
    std::ifstream file("levels/" + filename);
    if (!file.is_open())
    {
        std::cout << "ERROR: Failed to load level: " << filename << std::endl;
        return;
    }

    json data;
    file >> data;

    terrains.clear();

    auto generateCatmullRom =
        [](const std::vector<sf::Vector2f>& ctrl,
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

            pts.insert(pts.begin(), pts.front());
            pts.push_back(pts.back());

            for (size_t i = 0; i + 3 < pts.size(); ++i)
            {
                for (int s = 0; s < samplesPerSeg; ++s)
                {
                    float t = s / static_cast<float>(samplesPerSeg);
                    out.push_back(CR(
                        pts[i],
                        pts[i + 1],
                        pts[i + 2],
                        pts[i + 3],
                        t
                    ));
                }
            }

            out.push_back(ctrl.back());
            return out;
        };


    // main logic
    for (const auto& layer : data["layers"])
    {
		// ignre all layers that are not the "Collisions" object layer
        if (layer["type"] != "objectgroup" ||
            layer["name"] != "Collisions")
            continue;

        for (const auto& obj : layer["objects"])
        {
            float baseX = obj.value("x", 0.0f);
            float baseY = obj.value("y", 0.0f);

            bool isRect = !(obj.contains("polygon"));
			if (isRect) // RECTANGLE (just a simple box)
            {
                float w = obj["width"];
                float h = obj["height"];

                std::vector<sf::Vector2f> rectPoints =
                {
                    {baseX, baseY},
                    {baseX + w, baseY},
                    {baseX + w, baseY + h},
                    {baseX, baseY + h}
                };

                terrains.push_back(
                    std::make_unique<Terrain>(world, rectPoints)
                );
            }
			else // POLYGON
            {
				if (obj["polygon"].size() < 3)
                {
                    std::cout << "WARNING: Recieved a polygon with less than 3 points, polygon ignored." << std::endl;
                    continue;
                }

                std::vector<sf::Vector2f> polyPoints;
                for (const auto& p : obj["polygon"])
                {
                    polyPoints.emplace_back(
                        baseX + p["x"].get<float>(),
                        baseY + p["y"].get<float>()
                    );
                }

                if (obj["type"] == "concave") // concave, so need conversion to convex
                { 
                    auto convexPieces = ConvertConcaveToConvexPieces(polyPoints); // Convert to convex polygons
                    for (const auto& piece : convexPieces)
                    {
                        terrains.push_back(
                            std::make_unique<Terrain>(world, piece)
                        );
                    }
                }
				else { // else assume convex (so directly use the points to create a polygon)
                    terrains.push_back(
                        std::make_unique<Terrain>(world, polyPoints)
                    );
                }
            }
        }
    }
}

void Level::draw(sf::RenderWindow& window)
{
    for (auto& t : terrains)
        t->draw(window);
}