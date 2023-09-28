#ifndef MIRISTURTLE_H
#define MIRISTURTLE_H

#include <vector>
#include "json.hpp"

using json = nlohmann::json;

enum Field
{
    EMPTY,
    WALL,
    EXIT,
    START
};

/*Schildkroete Klasse*/
class Kroete
{
private:
    int position_x = 0;
    int position_y = 0;
    int direction = 2;
    int FIELDSIZE = 0;
    json path;
    // vector listen besser als arr, da ich Spielfeldgröße davor nicht festlegen will
    std::vector<std::vector<int>> spielfeld;

public:
    Kroete();
    ~Kroete();
    void moveForward();
    void turnLeft();
    void turnRight();
    bool isWallInFront();
    bool isWallRight();
    bool isWallLeft();
    bool imZiel();
};

#endif