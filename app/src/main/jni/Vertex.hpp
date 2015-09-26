//
// Created by Alexander Vlasov on 26.09.2015.
//

#ifndef PATHFINDING_VERTEX_H
#define PATHFINDING_VERTEX_H


#include <vector>
#include "Edge.hpp"

class Edge;

class Vertex {
    unsigned int row, column;
    unsigned long floor;
    float distance;
    bool visited;
    Vertex *previous;
    std::vector<Edge*> *edges;

public:

    Vertex();

    void SetPosition(unsigned int row, unsigned int column, unsigned long floor);

    void SetPrevious(Vertex *vertex);

    void SetVisited(bool visited);

    void SetDistance(float distance);

    void AddEdge(Edge *edge);

    bool IsVisited();

    unsigned int Row();

    unsigned int Column();

    unsigned long Floor();

    float Distance();

    Vertex* Previous();

    std::vector<Edge*>* Edges();
};

struct VertexComparator {
    bool operator() (Vertex *vertex, Vertex *vertex1) {
        return vertex->Distance() > vertex1->Distance();
    }
};


#endif //PATHFINDING_VERTEX_H
