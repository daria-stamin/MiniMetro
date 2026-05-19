#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class MetroLine;

// Represents one of the 6 fixed line slots shown in the bottom-right HUD.
// A slot is either free (no line) or occupied (has an active MetroLine).
class LineSlot
{
    sf::Color                  color;
    sf::Vector2f               hudPosition;   // centre of the circle in HUD
    std::shared_ptr<MetroLine> line;           // nullptr = slot is free

public:
    LineSlot(const sf::Color& color, const sf::Vector2f& hudPosition);

    // Assign / clear the active line for this slot
    void               setLine(std::shared_ptr<MetroLine> l);
    void               clearLine();

    bool               isFree()     const;
    sf::Color          getColor()   const;
    sf::Vector2f       getHudPosition() const;

    // non-owning access for SimulationEngine
    std::shared_ptr<MetroLine>       getLine();
    const std::shared_ptr<MetroLine> getLine() const;

    // Returns true if the given mouse position is inside this slot's circle
    bool containsPoint(const sf::Vector2f& point) const;

    void draw(sf::RenderWindow& window) const;
};