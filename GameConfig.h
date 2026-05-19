#pragma once

#include <string>
#include <stdexcept>

// Singleton pattern (Meyers Singleton — thread-safe in C++11+).
// Reads data/config.txt once and exposes typed getters.
// Constructor private, copy/move disabled.
class GameConfig
{
public:
    static GameConfig& instance()
    {
        static GameConfig cfg;   // constructed once, destroyed at program exit
        return cfg;
    }

    // Delete copy and move so there is truly only one instance
    GameConfig(const GameConfig&)            = delete;
    GameConfig& operator=(const GameConfig&) = delete;
    GameConfig(GameConfig&&)                 = delete;
    GameConfig& operator=(GameConfig&&)      = delete;

    int   maxPassengersPerStation()   const { return maxPassengers;         }
    float stationSpawnInterval()      const { return spawnInterval;         }
    float trainSpeed()                const { return trainSpd;              }
    float overcrowdWarningDuration()  const { return warningDuration;       }

private:
    int   maxPassengers   = 10;
    float spawnInterval   = 30.f;
    float trainSpd        = 0.4f;
    float warningDuration = 2.f;

    GameConfig();   // loads config.txt

    void parseLine(const std::string& line);
};