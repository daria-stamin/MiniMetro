#pragma once

#include <SFML/Graphics.hpp>
#include "SimulationEngine.h"

class GameRenderer
{
public:
    void render(
        sf::RenderWindow& window,
        const SimulationEngine& engine,
        const sf::Font& font,
        float totalTime
    );
};