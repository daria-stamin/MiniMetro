#pragma once

#include <memory>
#include "Station.h"

// Factory Method pattern.
// Centralizes creation of Station subclasses so no caller ever
// needs to #include all three concrete headers or switch on StationType.
class StationFactory
{
public:
    // Not instantiable — all methods are static
    StationFactory() = delete;

    static std::shared_ptr<Station> create(
        StationType type,
        const sf::Vector2f& position
    );
};