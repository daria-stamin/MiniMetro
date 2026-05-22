#include "CircleStation.h"

CircleStation::CircleStation(const sf::Vector2f& pos): Station(pos) {}

StationType CircleStation::getType() const { return StationType::Circle; }

void CircleStation::draw(sf::RenderWindow& window) const{
    sf::CircleShape shape(14.f);
    shape.setFillColor(sf::Color(250, 250, 250));
    shape.setOutlineThickness(5.f);
    shape.setOutlineColor(sf::Color::Black);
    shape.setOrigin(14.f, 14.f);
    shape.setPosition(getPosition());
    window.draw(shape);
}