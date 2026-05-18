cmake_minimum_required(VERSION 3.24)
project(MiniMetro)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(MiniMetro main.cpp
        Station.cpp
        Station.h
        MetroLine.cpp
        MetroLine.h
        Train.cpp
        Train.h
        Passenger.cpp
        Passenger.h
        SimulationEngine.cpp
        SimulationEngine.h
        GameRenderer.cpp
        GameRenderer.h
        CircleStation.cpp
        CircleStation.h
        SquareStation.cpp
        SquareStation.h
        TriangleStation.cpp
        TriangleStation.h
        Stationfactory.cpp
        Stationfactory.h)
target_link_libraries(MiniMetro sfml-graphics sfml-window sfml-system)

# =========================================================================
# SOLUȚIA CORECTATĂ PENTRU COPIEREA DLL-URILOR
# =========================================================================
if(WIN32)
    add_custom_command(TARGET MiniMetro POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            "$<TARGET_FILE_DIR:MiniMetro>" # Am schimbat în TARGET_FILE_DIR și am pus ghilimele
    )
endif()