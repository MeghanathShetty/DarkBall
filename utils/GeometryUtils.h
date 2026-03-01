#pragma once
#include <SFML/System.hpp>
#include <vector>

std::vector<std::vector<sf::Vector2f>>
ConvertConcaveToConvexPieces(const std::vector<sf::Vector2f>& polygon);