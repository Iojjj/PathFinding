//
// Created by Alexander Vlasov on 26.09.2015.
//

#ifndef PATHFINDING_EDGE_H
#define PATHFINDING_EDGE_H


#include "Vertex.hpp"

class Vertex;

class Edge {
    Vertex *vertexTo;
    float cost;

public:

    Edge(Vertex *vertexTo, const float cost);

    Vertex* VertexTo();

    float Cost();
};


#endif //PATHFINDING_EDGE_H
