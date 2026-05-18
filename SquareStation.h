#pragma once
#include "Station.h"

class SquareStation final : public Station
{
public:
    explicit SquareStation(const sf::Vector2f& position);

    StationType getType() const override;
    void draw(sf::RenderWindow& window) const override;
};