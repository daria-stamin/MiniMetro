#pragma once
#include "Passenger.h"
#include <vector>

class Train
{
    int   currentStationIndex;
    int   nextStationIndex;
    float progress;
    float speed;
    bool  movingForward;

    std::vector<Passenger> passengers;

public:
    Train();

    void update(int stationCount, float dt);

    int   getCurrentStationIndex() const;
    int   getNextStationIndex()    const;
    float getProgress()            const;

    // non-const — used by MetroLine for boarding/alighting
    std::vector<Passenger>& getPassengers();

    // const — used by GameRenderer (read-only)
    const std::vector<Passenger>& getPassengers() const;
};