//
// Created by Alexander Vlasov on 26.09.2015.
//

#include "Edge.hpp"

Edge::Edge(Vertex *vertexTo, const float cost) {
    this->vertexTo = vertexTo;
    this->cost = cost;
}

Vertex *Edge::VertexTo() {
    return vertexTo;
}

float Edge::Cost() {
    return cost;
}
