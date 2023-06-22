#ifndef MIRISTURTLE_H
#define MIRISTURTLE_H
#include <fstream>
#include <vector>

enum Field {
    EMPTY,
    WALL,
    EXIT,
    START
};

/*Schildkroete Klasse*/
class Kroete{
private:
    int position_x = 0;
    int position_y = 0;
    int direction = 2;
    std::ofstream path;
    //vector listen besser als arr, da ich spielfeldgröße davor nicht festlegen will
    std::vector<std::vector<int>> spielfeld;
public:
    Kroete();
    ~Kroete();
    void moveForward();
    void turnLeft();
    void turnRight();
    bool isWallInFront();
};

#endif