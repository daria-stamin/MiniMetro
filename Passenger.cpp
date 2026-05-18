#include "Passenger.h"

Passenger::Passenger(StationType type)
{
    destinationType = type;
}

StationType Passenger::getDestinationType() const
{
    return destinationType;
}