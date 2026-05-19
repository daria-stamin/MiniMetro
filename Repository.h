#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>

// ── Concept: HasPosition ─────────────────────────────────────────────────────
// Constrains findNearest to only work with types that have getPosition().
// C++20 feature — gives a clear compiler error if T doesn't qualify.
template<typename T>
concept HasPosition = requires(const T& obj)
{
    { obj.getPosition() } -> std::convertible_to<sf::Vector2f>;
};

// ── Repository<T> ────────────────────────────────────────────────────────────
// Generic container for shared_ptr<T>.
// Used with T=Station and T=MetroLine in SimulationEngine.
template<typename T>
class Repository
{
    std::vector<std::shared_ptr<T>> items;

public:
    void add(std::shared_ptr<T> item)
    {
        items.push_back(std::move(item));
    }

    const std::vector<std::shared_ptr<T>>& getAll() const
    {
        return items;
    }

    std::size_t count() const { return items.size(); }

    // Apply a function to every element — replaces raw for loops
    void forEach(std::function<void(T&)> fn)
    {
        for (auto& ptr : items)
            fn(*ptr);
    }

    // Find first element matching predicate — returns nullopt if not found
    std::optional<std::shared_ptr<T>> findIf(
        std::function<bool(const T&)> predicate) const
    {
        auto it = std::find_if(items.begin(), items.end(),
            [&](const std::shared_ptr<T>& ptr) {
                return predicate(*ptr);
            });
        return it != items.end() ? std::optional{*it} : std::nullopt;
    }

    // Count elements matching predicate
    void remove(const std::shared_ptr<T>& item)
    {
        items.erase(
            std::remove(items.begin(), items.end(), item),
            items.end());
    }

    int countIf(std::function<bool(const T&)> predicate) const
    {
        return static_cast<int>(
            std::count_if(items.begin(), items.end(),
                [&](const std::shared_ptr<T>& ptr) {
                    return predicate(*ptr);
                }));
    }
};

// ── Free template function: findNearest ──────────────────────────────────────
// Only compiles for types with getPosition() — enforced by HasPosition concept.
// Returns optional so caller never gets a null pointer.
template<HasPosition T>
std::optional<std::shared_ptr<T>> findNearest(
    const Repository<T>& repo,
    const sf::Vector2f&  point,
    float                maxDistance)
{
    std::optional<std::shared_ptr<T>> best;
    float bestDist = maxDistance;

    for (const auto& item : repo.getAll())
    {
        sf::Vector2f pos  = item->getPosition();
        float dist = std::sqrt(
            std::pow(point.x - pos.x, 2.f) +
            std::pow(point.y - pos.y, 2.f));

        if (dist < bestDist)
        {
            bestDist = dist;
            best     = item;
        }
    }
    return best;
}