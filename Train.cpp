#include "Train.h"

Train::Train()
    : currentStationIndex(0)
    , nextStationIndex(1)
    , progress(0.f)
    , speed(0.4f)
    , movingForward(true)
{}

void Train::update(int stationCount, float dt)
{
    progress += speed * dt;

    while (progress >= 1.f)
    {
        progress -= 1.f;

        if (movingForward) { currentStationIndex++; nextStationIndex++; }
        else               { currentStationIndex--; nextStationIndex--; }

        if (nextStationIndex >= stationCount)
        {
            movingForward = false;
            currentStationIndex = stationCount - 1;
            nextStationIndex    = stationCount - 2;
        }

        if (nextStationIndex < 0)
        {
            movingForward = true;
            currentStationIndex = 0;
            nextStationIndex    = 1;
        }
    }
}

int   Train::getCurrentStationIndex() const { return currentStationIndex; }
int   Train::getNextStationIndex()    const { return nextStationIndex;    }
float Train::getProgress()            const { return progress;            }

std::vector<Passenger>&       Train::getPassengers()       { return passengers; }
const std::vector<Passenger>& Train::getPassengers() const { return passengers; }