#include "turtle.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <fstream>

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
    // Lade Spielfeld
    std::ifstream labyrinthdatei("labyrinth.json");

    if (!labyrinthdatei)
    {
        std::cerr << "Datei wurde nicht gefunden!";
        return;
    }
    // .json in in json Objekt laden
    json data = json::parse(labyrinthdatei);
    FIELDSIZE = data.size();

    // Labyrinth in das Vektorobjekt "spielfeld"  laden
    // durchläuft jede Zeile des Labyrinths
    for (int i = 0; i < FIELDSIZE; i++){
    //leere Vektorreihe "row" erstellen
        std::vector<int> row;
    //durchläuft jede Spalte des Labyrinths    
        for (int j = 0; j < FIELDSIZE; j++)
        {
    //fügt den Wert aus "data[i][j]" dem Vektor "row" hinzu
            row.push_back(data[i][j]);
        }
    //fügt die fertige "row" dem Vektor "spielfeld" hinzu
        spielfeld.push_back(row);
    }
    path = json::array();

}

Kroete::~Kroete()
{ // path in datei
    std::ofstream pathfile("kroeten_path.json");
    pathfile << path;
}

/*Funktionen*/
// Ist Wand vor mir
bool Kroete::isWallInFront()
{
    // Prüft die Ausrichtung der Schildkröte
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

/*Bewegungsfunktionen */
// Bewege mich vorwärts
void Kroete::moveForward()
{
    if (!isWallInFront())
    {
        switch (direction)
        {
        case 0:
            path.push_back({{"start_x", position_x}, {"start_y", position_y}, {"end_x", position_x}, {"end_y", position_y - 1}, {"direction", direction}});
            position_y -= 1;
            break;

        case 1:
            path.push_back({{"start_x", position_x}, {"start_y", position_y}, {"end_x", position_x + 1}, {"end_y", position_y}, {"direction", direction}});
            position_x += 1;
            break;

        case 2:
            path.push_back({{"start_x", position_x}, {"start_y", position_y}, {"end_x", position_x}, {"end_y", position_y + 1}, {"direction", direction}});
            position_y += 1;
            break;

        case 3:
            path.push_back({{"start_x", position_x}, {"start_y", position_y}, {"end_x", position_x - 1}, {"end_y", position_y}, {"direction", direction}});
            position_x -= 1;
            break;
        }
    }
    else
    {
        std::cout << "Du kannst hier nicht hinkröten, weil hier eine Wand ist! " << std::endl;
        std::cout << "Probiere es nochmal!" << std::endl;

        std::ofstream pathfile("kroeten_path.json");
        pathfile << path;
        exit(0);
    }
}

// Drehe mich nach links
void Kroete::turnLeft()
{
    //subtrahiere 1 um Ausrichtung 90 Grad nach links zu drehen
    // vier mögliche Ausrichtungen -> Division modulo 4 , um im Bereich von 0 bis 3 zu bleiben
    int newDirection = (direction - 1) % 4;
    // wenn norden 0 -> -1, dann auf 3 gesetzt
    if (newDirection == -1)
    {
        direction = 3;
    }
    // sonst neue Ausrichtung
    else
    {
        direction = newDirection;
    }

    path.push_back({{"start_x", position_x}, {"start_y", position_y}, {"end_x", position_x}, {"end_y", position_y}, {"direction", direction}});
}

// Drehe mich nach rechts
void Kroete::turnRight()
{
    //addiere 1 um Ausrichtung 90 Grad nach rechts zu drehen
    // vier mögliche Ausrichtungen -> Division modulo 4 , um im Bereich von 0 bis 3 zu bleiben
    direction = abs(direction + 1) % 4;
    path.push_back({{"start_x", position_x}, {"start_y", position_y}, {"end_x", position_x}, {"end_y", position_y}, {"direction", direction}});
}