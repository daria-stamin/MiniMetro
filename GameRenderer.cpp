#include "GameRenderer.h"
#include "GameConfig.h"
#include <cmath>

// ── Passenger icon helper ─────────────────────────────────────────────────
static void drawPassengerIcon(
    sf::RenderWindow& window,
    StationType type,
    const sf::Vector2f& center,
    float size,
    sf::Color color)
{
    if (type == StationType::Circle)
    {
        sf::CircleShape s(size);
        s.setFillColor(color); s.setOrigin(size, size); s.setPosition(center);
        window.draw(s);
    }
    else if (type == StationType::Square)
    {
        sf::RectangleShape s(sf::Vector2f(size * 2.f, size * 2.f));
        s.setFillColor(color); s.setOrigin(size, size); s.setPosition(center);
        window.draw(s);
    }
    else
    {
        sf::CircleShape s(size * 1.2f, 3);
        s.setFillColor(color); s.setOrigin(size * 1.2f, size * 1.2f); s.setPosition(center);
        window.draw(s);
    }
}

// ── Score overlay (top-left) ─────────────────────────────────────────────────
static void drawScore(
    sf::RenderWindow& window,
    const sf::Font& font,
    int score)
{
    sf::Text text;
    text.setFont(font);
    text.setString("score: " + std::to_string(score));
    text.setCharacterSize(22);
    text.setFillColor(sf::Color(30, 30, 30));
    text.setPosition(16.f, 12.f);
    window.draw(text);
}

// ── Game Over screen ─────────────────────────────────────────────────────────
static void drawGameOver(
    sf::RenderWindow& window,
    const sf::Font& font,
    int score)
{
    // Semi-transparent dark overlay
    sf::RectangleShape overlay(sf::Vector2f(1280.f, 720.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text title;
    title.setFont(font);
    title.setString("GAME OVER");
    title.setCharacterSize(72);
    title.setFillColor(sf::Color(220, 50, 50));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(640.f, 300.f);
    window.draw(title);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Passengers delivered: " + std::to_string(score));
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    sf::FloatRect sb = scoreText.getLocalBounds();
    scoreText.setOrigin(sb.width / 2.f, sb.height / 2.f);
    scoreText.setPosition(640.f, 400.f);
    window.draw(scoreText);

    sf::Text hint;
    hint.setFont(font);
    hint.setString("Close the window to exit");
    hint.setCharacterSize(22);
    hint.setFillColor(sf::Color(180, 180, 180));
    sf::FloatRect hb = hint.getLocalBounds();
    hint.setOrigin(hb.width / 2.f, hb.height / 2.f);
    hint.setPosition(640.f, 470.f);
    window.draw(hint);
}

// ── Main render ──────────────────────────────────────────────────────────────
void GameRenderer::render(
    sf::RenderWindow& window,
    const SimulationEngine& engine,
    const sf::Font& font,
    float totalTime)
{
    // ── Lines ────────────────────────────────────────────────────────────
    for (const auto& line : engine.getMetroLines())
    {
        const auto& stations = line->getStations();
        if (stations.size() < 2) continue;

        for (size_t i = 0; i < stations.size() - 1; ++i)
        {
            sf::Vector2f start = stations[i]->getPosition();
            sf::Vector2f end   = stations[i + 1]->getPosition();
            sf::Vector2f diff  = end - start;

            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            float angle  = std::atan2(diff.y, diff.x) * 180.f / 3.14159265f;

            sf::RectangleShape lineShape(sf::Vector2f(length, 6.f));
            lineShape.setFillColor(line->getColor());
            lineShape.setPosition(start);
            lineShape.setRotation(angle);
            lineShape.setOrigin(0.f, 3.f);
            window.draw(lineShape);
        }
    }

    // ── Trains + passengers ───────────────────────────────────────────────
    for (const auto& line : engine.getMetroLines())
    {
        const auto& lineStations = line->getStations();
        if (lineStations.size() < 2) continue;

        for (const auto& train : line->getTrains())
        {
            sf::Vector2f start = lineStations[train.getCurrentStationIndex()]->getPosition();
            sf::Vector2f end   = lineStations[train.getNextStationIndex()]->getPosition();

            float t = train.getProgress();
            t = t * t * (3.f - 2.f * t);
            sf::Vector2f trainPos = start + (end - start) * t;

            sf::Vector2f dir = end - start;
            float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;

            const float W = 48.f, H = 18.f;
            sf::RectangleShape body(sf::Vector2f(W, H));
            body.setFillColor(line->getColor());
            body.setOrigin(W / 2.f, H / 2.f);
            body.setPosition(trainPos);
            body.setRotation(angle);
            window.draw(body);

            const auto& passengers = train.getPassengers();
            const size_t count = std::min(passengers.size(), size_t(6));
            float rad  = angle * 3.14159265f / 180.f;
            float cosA = std::cos(rad), sinA = std::sin(rad);
            float colOff[3] = { -14.f, 0.f, 14.f };
            float rowOff[2] = {  -4.f, 4.f };

            for (size_t i = 0; i < count; ++i)
            {
                float lx = colOff[i % 3], ly = rowOff[i / 3];
                sf::Vector2f iconPos = trainPos + sf::Vector2f(
                    lx * cosA - ly * sinA,
                    lx * sinA + ly * cosA);
                drawPassengerIcon(window, passengers[i].getDestinationType(),
                                  iconPos, 3.f, sf::Color(255, 255, 255, 220));
            }
        }
    }

    // ── Stations ─────────────────────────────────────────────────────────
    for (const auto& station : engine.getStations())
    {
        // Blink effect: when overcrowded, alternate between red tint and white
        // using a sine wave on the overcrowd timer.
        if (station->isOvercrowded())
        {
            float blink = std::sin(station->getOvercrowdTimer() * 10.f);
            if (blink > 0.f)
            {
                // Draw a red glow circle behind the station
                sf::CircleShape glow(24.f);
                glow.setFillColor(sf::Color(220, 50, 50, 180));
                glow.setOrigin(24.f, 24.f);
                glow.setPosition(station->getPosition());
                window.draw(glow);
            }
        }

        station->draw(window);   // virtual dispatch — no if/else needed

        // Waiting passengers
        const auto& passengers = station->getPassengers();
        for (size_t i = 0; i < passengers.size(); ++i)
        {
            sf::Vector2f iconPos(
                station->getPosition().x + 24.f + i * 14.f,
                station->getPosition().y);
            drawPassengerIcon(window, passengers[i].getDestinationType(),
                              iconPos, 5.f, sf::Color::Black);
        }
    }

    // ── Live score ────────────────────────────────────────────────────────
    drawScore(window, font, engine.getScore());

    // ── Overcrowded station count (uses countIf<Station> internally) ──────
    int overcrowded = engine.getOvercrowdedStationCount();
    if (overcrowded > 0)
    {
        sf::Text warn;
        warn.setFont(font);
        warn.setString("overcrowded stations: " + std::to_string(overcrowded));
        warn.setCharacterSize(22);
        warn.setFillColor(sf::Color(200, 50, 50));
        warn.setPosition(16.f, 40.f);
        window.draw(warn);
    }

    // ── Line slots HUD (bottom-right) ────────────────────────────────────
    for (const auto& slot : engine.getSlots())
        slot.draw(window);

    // ── Game over screen (drawn last, on top of everything) ───────────────
    if (engine.isGameOver())
        drawGameOver(window, font, engine.getScore());
}