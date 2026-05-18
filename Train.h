#pragma once
#include "Passenger.h"
#include <vector>
class Train
{
private:
    int currentStationIndex;

    int nextStationIndex;

    float progress;

    float speed;

    bool movingForward;
    std::vector<Passenger> passengers;

public:
    Train();

    void update(int stationCount, float dt);

    int getCurrentStationIndex() const;

    int getNextStationIndex() const;

    float getProgress() const;
    std::vector<Passenger>&getPassengers();

};