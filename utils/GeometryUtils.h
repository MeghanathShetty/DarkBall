#pragma once
#include <SFML/System.hpp>
#include <vector>

std::vector<std::vector<sf::Vector2f>>
EarClipTriangulate(const std::vector<sf::Vector2f>& polygon);

std::vector<sf::Vector2f>
GenerateCatmullRom(const std::vector<sf::Vector2f>& controlPoints,
    int samplesPerSegment);