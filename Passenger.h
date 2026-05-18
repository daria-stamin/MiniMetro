#pragma once

enum class StationType
{
    Circle,
    Square,
    Triangle
};

class Passenger
{
private:
    StationType destinationType;

public:
    Passenger(StationType destinationType);

    StationType getDestinationType() const;
};

