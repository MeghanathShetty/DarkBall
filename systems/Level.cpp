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

	// Iterate through all layers in the Tiled map data
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

				// Smooth the polygon points using Catmull-Rom
                const int samplesPerSegment = 5; // Keep small. 4–6 is enough.
                std::vector<sf::Vector2f> smoothPoints;
                if (polyPoints.size() >= 4)
                {
                    smoothPoints = GenerateCatmullRom(polyPoints, samplesPerSegment);
                }
                else
                {
                    // Too small to smooth safely
                    smoothPoints = polyPoints;
                }

                // Convert polygons into smaller chunks of triangles 
				// Box2D only supports 3 to 8 vertices per polygon and also only convex shapes
                // so Ear Clipping handles these both issues
                auto convexPieces = EarClipTriangulate(smoothPoints); // Convert to convex polygons
                for (const auto& piece : convexPieces)
                {
                    terrains.push_back(
                        std::make_unique<Terrain>(world, piece)
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