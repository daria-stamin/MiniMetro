#include "StationFactory.h"
#include "CircleStation.h"
#include "SquareStation.h"
#include "TriangleStation.h"
#include <stdexcept>

std::shared_ptr<Station> StationFactory::create(
    StationType type,
    const sf::Vector2f& position)
{
    switch (type)
    {
        case StationType::Circle:
            return std::make_shared<CircleStation>(position);
        case StationType::Square:
            return std::make_shared<SquareStation>(position);
        case StationType::Triangle:
            return std::make_shared<TriangleStation>(position);
        default:
            throw std::invalid_argument("Unknown StationType in StationFactory::create");
    }
}