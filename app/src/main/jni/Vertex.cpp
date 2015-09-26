//
// Created by Alexander Vlasov on 26.09.2015.
//

#include "Vertex.hpp"
#include "Log.hpp"

Vertex::Vertex() {
    row = column = 0;
    previous = nullptr;
    distance = 0;
    visited = false;
    edges = new std::vector<Edge*>;
}

void Vertex::SetPosition(unsigned int row, unsigned int column, unsigned long floor) {
    this->row = row;
    this->column = column;
    this->floor = floor;
}

void Vertex::SetPrevious(Vertex *vertex) {
    previous = vertex;
}

void Vertex::SetVisited(bool visited) {
    this->visited = visited;
}

void Vertex::SetDistance(float distance) {
    this->distance = distance;
}

void Vertex::AddEdge(Edge *edge) {
    edges->push_back(edge);
}

bool Vertex::IsVisited() {
    return visited;
}

unsigned int Vertex::Row() {
    return row;
}

unsigned int Vertex::Column() {
    return column;
}

Vertex *Vertex::Previous() {
    return previous;
}

std::vector<Edge*>* Vertex::Edges() {
    return edges;
}

float Vertex::Distance() {
    return distance;
}

unsigned long Vertex::Floor() {
    return floor;
}
