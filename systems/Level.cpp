#include "Level.h"
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
        std::cout << "Failed to load: " << filename << std::endl;
        return;
    }

    json data;
    file >> data;

    terrains.clear();

    const int samplesPerSegment = 16;

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

    for (const auto& layer : data["layers"])
    {
        if (layer["type"] != "objectgroup" ||
            layer["name"] != "Collisions")
            continue;

        for (const auto& obj : layer["objects"])
        {
            float baseX = obj.value("x", 0.0f);
            float baseY = obj.value("y", 0.0f);

            // RECTANGLE = straight box
            if (obj.contains("width") && obj.contains("height") &&
                !obj.contains("polygon") &&
                !obj.contains("polyline"))
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

            // POLYLINE = smooth slope
            else if (obj.contains("polyline"))
            {
                std::vector<sf::Vector2f> controlPoints;

                for (const auto& p : obj["polyline"])
                {
                    controlPoints.emplace_back(
                        baseX + p["x"].get<float>(),
                        baseY + p["y"].get<float>()
                    );
                }

                if (controlPoints.size() >= 2)
                {
                    auto smooth =
                        generateCatmullRom(controlPoints,
                            samplesPerSegment);

                    terrains.push_back(
                        std::make_unique<Terrain>(world, smooth)
                    );
                }
            }

            // POLYGON = straight edges (no smoothing)
            else if (obj.contains("polygon"))
            {
                std::vector<sf::Vector2f> polyPoints;

                for (const auto& p : obj["polygon"])
                {
                    polyPoints.emplace_back(
                        baseX + p["x"].get<float>(),
                        baseY + p["y"].get<float>()
                    );
                }

                terrains.push_back(
                    std::make_unique<Terrain>(world, polyPoints)
                );
            }
        }
    }
}

void Level::draw(sf::RenderWindow& window)
{
    for (auto& t : terrains)
        t->draw(window);
}