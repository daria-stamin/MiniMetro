#pragma once
#include "Station.h"

class CircleStation final : public Station
{
public:
    explicit CircleStation(const sf::Vector2f& position);

    StationType getType() const override;
    void draw(sf::RenderWindow& window) const override;
};