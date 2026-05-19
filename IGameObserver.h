#pragma once

// Observer interface.
// Station (Subject) calls onStationOvercrowded() when its passenger
// count reaches the configured limit.
class Station;  // forward declaration — observer doesn't need full type

class IGameObserver
{
public:
    virtual ~IGameObserver() = default;
    virtual void onStationOvercrowded(Station* station) = 0;
};