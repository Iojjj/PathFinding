#ifndef PATHFINDING_DIJKSTRA_H
#define PATHFINDING_DIJKSTRA_H

#include <cfloat>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <map>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Transit.hpp"

class Dijkstra {
private:
    std::map<unsigned long, std::vector<Vertex*>> floorsMap;
    std::map<unsigned long, std::pair<unsigned int, unsigned int>> floorsSizes;

    void ClearDistances();

    void CalculateDistances(Vertex *vertex);

public:
    Dijkstra();

    void AddFloor(unsigned long floorId, unsigned int rows, unsigned int columns, std::vector<Vertex *> *vertices, float costStraight, float costDiagonal);

    void AddTransits(std::list<Transit *> transits, float costTransit);

    std::list<unsigned int*> FindPath(Transit *transit);

    ~Dijkstra();
};


#endif //PATHFINDING_DIJKSTRA_H
