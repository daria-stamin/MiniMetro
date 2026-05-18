#include "TriangleStation.h"

TriangleStation::TriangleStation(const sf::Vector2f& pos)
    : Station(pos) {}

StationType TriangleStation::getType() const { return StationType::Triangle; }

void TriangleStation::draw(sf::RenderWindow& window) const
{
    sf::CircleShape shape(18.f, 3);
    shape.setFillColor(sf::Color(250, 250, 250));
    shape.setOutlineThickness(5.f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOrigin(18.f, 18.f);
    shape.setPosition(getPosition());
    window.draw(shape);
}