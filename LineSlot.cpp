#include "LineSlot.h"
#include "MetroLine.h"
#include <cmath>

static constexpr float RADIUS = 18.f;

LineSlot::LineSlot(const sf::Color& color, const sf::Vector2f& pos)
    : color(color), hudPosition(pos) {}

void LineSlot::setLine(std::shared_ptr<MetroLine> l) { line = std::move(l); }
void LineSlot::clearLine()                            { line = nullptr;      }

bool                             LineSlot::isFree()        const { return line == nullptr; }
sf::Color                        LineSlot::getColor()       const { return color;           }
sf::Vector2f                     LineSlot::getHudPosition() const { return hudPosition;     }
std::shared_ptr<MetroLine>       LineSlot::getLine()              { return line;            }
const std::shared_ptr<MetroLine> LineSlot::getLine()        const { return line;            }

bool LineSlot::containsPoint(const sf::Vector2f& point) const
{
    float dx = point.x - hudPosition.x;
    float dy = point.y - hudPosition.y;
    return std::sqrt(dx * dx + dy * dy) <= RADIUS;
}

void LineSlot::draw(sf::RenderWindow& window) const
{
    sf::CircleShape circle(RADIUS);
    circle.setOrigin(RADIUS, RADIUS);
    circle.setPosition(hudPosition);
    circle.setOutlineThickness(0.f);  // no outline ever

    if (isFree())
    {
        // Faded version of the color — slot is available but unused
        sf::Color faded = color;
        faded.a = 80;
        circle.setFillColor(faded);
    }
    else
    {
        // Full solid color — line is active
        circle.setFillColor(color);
    }

    window.draw(circle);
}