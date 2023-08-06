#include "turtle.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

const int FIELDSIZE = 10;
/*spielfeld = {
    {3, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 1, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 1, 1, 0, 1},
    {0, 1, 0, 1, 0, 0, 0, 0, 0, 2}
};*/
Kroete::Kroete()
{
    // labyrinthdatei so aufbrechen, dass ich einzelene zahlen für jede reihe übergebe (matrix)
    std::ifstream labyrinthdatei("labyrinth.txt");
    std::string line;
    if (labyrinthdatei.is_open())
    {
        // lese datei zeile für zeile
        // 3,1,1,1,0,0,0,0,0,0;0,1,0,1,0,1,0,0,0,0;
        while (getline(labyrinthdatei, line))
        {
            std::stringstream ss(line);
            std::string row;
            // ss wir bei ; gespilted
            //'3,1,1,1,0,0,0,0,0,0', '0,1,0,1,0,1,0,0,0,0'
            while (std::getline(ss, row, ';'))
            {
                std::vector<int> rowvalue;
                std::stringstream rowstream(row);
                std::string value;
                // row wird bei , gesplitted
                //[['3', '1', '1', '1', '0', '0', '0', '0', '0', '0'],['0', '1', '0', '1', '0', '1', '0', '0', '0', '0'],
                while (std::getline(rowstream, value, ','))
                {
                    // aus string wird ein int
                    rowvalue.push_back(std::stoi(value));
                }
                // zahlen kommen in das spielfeld
                spielfeld.push_back(rowvalue);
            }
        }
        labyrinthdatei.close();
    }
    path = std::ofstream("kroeten_path.json");
    if (!path)
    {
        std::cerr << "Datei wurde nicht erstellt!";
    }
    path << "[";
}

Kroete::~Kroete()
{
    path << "]";
    path.close();
}

/*Funktionen*/
// Ist Wand vor mir
bool Kroete::isWallInFront()
{
    switch (direction)
    {
    // north
    case 0:
        return (position_y - 1) < 0 || spielfeld[position_y - 1][position_x] == WALL;
        break;
    // east
    case 1:
        return (position_x + 1) >= FIELDSIZE || spielfeld[position_y][position_x + 1] == WALL;
        break;
    // south
    case 2:
        return (position_y + 1) >= FIELDSIZE || spielfeld[position_y + 1][position_x] == WALL;
        break;
    // west
    case 3:
        return (position_x - 1) < 0 || spielfeld[position_y][position_x - 1] == WALL;
        break;

    default:
        return false;
    }
}

// Ist Wand rechts von mir
bool Kroete::isWallRight()
{
    switch (direction)
    {
    // schaue nach norden, wand osten
    case 0:
        return (position_x + 1) >= FIELDSIZE || spielfeld[position_y][position_x + 1] == WALL;
        break;
    // schaue nach osten, wand süden
    case 1:
        return (position_y + 1) >= FIELDSIZE || spielfeld[position_y + 1][position_x] == WALL;
        break;
    // schaue nach süden, wand westen
    case 2:
        return (position_x - 1) < 0 || spielfeld[position_y][position_x - 1] == WALL;
        break;
    // schaue nach westen, wand norden
    case 3:
        return (position_y - 1) < 0 || spielfeld[position_y - 1][position_x] == WALL;
        break;

    default:
        return false;
    }
}

// Ist Wand links von mir
bool Kroete::isWallLeft()
{
    switch (direction)
    {
    // schaue nach norden, wand westen
    case 0:
        return (position_x - 1) >= FIELDSIZE || spielfeld[position_y][position_x - 1] == WALL;
        break;
    // schaue nach osten, wand norden
    case 1:
        return (position_y - 1) >= FIELDSIZE || spielfeld[position_y - 1][position_x] == WALL;
        break;
    // schaue nach süden, wand osten
    case 2:
        return (position_x + 1) < 0 || spielfeld[position_y][position_x + 1] == WALL;
        break;
    // schaue nach westen, wand norden
    case 3:
        return (position_y + 1) < 0 || spielfeld[position_y + 1][position_x] == WALL;
        break;

    default:
        return false;
    }
}

// Im Ziel?
bool Kroete::imZiel()
{
    return spielfeld[position_y][position_x] == EXIT;
}

void Kroete::moveForward()
{
    if (!isWallInFront())
    {
        /* \"start"\ : wird in json nur "start": sein*/
        switch (direction)
        {
        case 0:
            path << "{\"start_x\":" << position_x << ", \"start_y\":" << position_y << ", \"end_x\":" << position_x << ", \"end_y\":" << position_y - 1 << ", \"direction\":" << direction << "},";
            position_y -= 1;
            break;

        case 1:
            path << "{\"start_x\":" << position_x << ", \"start_y\":" << position_y << ", \"end_x\":" << position_x + 1 << ", \"end_y\":" << position_y << ", \"direction\":" << direction << "},";
            position_x += 1;
            break;

        case 2:
            path << "{\"start_x\":" << position_x << ", \"start_y\":" << position_y << ", \"end_x\":" << position_x << ", \"end_y\":" << position_y + 1 << ", \"direction\":" << direction << "},";
            position_y += 1;
            break;

        case 3:
            path << "{\"start_x\":" << position_x << ", \"start_y\":" << position_y << ", \"end_x\":" << position_x - 1 << ", \"end_y\":" << position_y << ", \"direction\":" << direction << "},";
            position_x -= 1;
            break;
        }
    }
    else
    {
        std::cout << "Du kannst hier nicht hinkröten, weil hier eine Wand ist! " << std::endl;
        std::cout << "Probiere es nochmal!" << std::endl;
        path << "]";
        path.close();
        exit(0);
    }
}

void Kroete::turnLeft()
{
    int newDirection = (direction - 1) % 4;
    if (newDirection == -1)
    {
        direction = 3;
    }
    else
    {
        direction = newDirection;
    }
    path << "{\"start_x\":" << position_x << ", \"start_y\":" << position_y << ", \"end_x\":" << position_x << ", \"end_y\":" << position_y << ", \"direction\":" << direction << "},";
}

void Kroete::turnRight()
{
    direction = abs(direction + 1) % 4;
    path << "{\"start_x\":" << position_x << ", \"start_y\":" << position_y << ", \"end_x\":" << position_x << ", \"end_y\":" << position_y << ", \"direction\":" << direction << "},";
}