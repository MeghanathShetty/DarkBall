#pragma once
#include <memory>
#include "Terrain.h"

class Level
{
public:
    void load(const std::string& filename, b2World& world);
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::unique_ptr<Terrain>> terrains;
};