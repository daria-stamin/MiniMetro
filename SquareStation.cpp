#include "SquareStation.h"

SquareStation::SquareStation(const sf::Vector2f& pos)
    : Station(pos) {}

StationType SquareStation::getType() const { return StationType::Square; }

void SquareStation::draw(sf::RenderWindow& window) const
{
    sf::RectangleShape shape(sf::Vector2f(28.f, 28.f));
    shape.setFillColor(sf::Color(250, 250, 250));
    shape.setOutlineThickness(5.f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOrigin(14.f, 14.f);
    shape.setPosition(getPosition());
    window.draw(shape);
}