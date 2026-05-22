#pragma once
#include "Station.h"

class TriangleStation final : public Station
{
public:
     TriangleStation(const sf::Vector2f& position);

    StationType getType() const override;
    void draw(sf::RenderWindow& window) const override;
};