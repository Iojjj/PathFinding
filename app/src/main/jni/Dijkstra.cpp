//
// Created by Alexander Vlasov on 21.09.2015.
//

#include "Dijkstra.hpp"
#include "Log.hpp"

#define TAG "Dijkstra"

Dijkstra::Dijkstra() {
    LOGD(TAG, "constructor");
};

Dijkstra::~Dijkstra() {
    LOGD(TAG, "destructor", nullptr);
    std::map<unsigned long, std::vector<Vertex *> *>::iterator it;
    std::vector<Vertex *>::iterator iterator;
    for (auto pair : floorsMap) {
        for (iterator = pair.second.begin(); iterator != pair.second.end(); iterator++) {
            delete *iterator;
        }
        pair.second.clear();
    }
    floorsMap.clear();
    floorsSizes.clear();
};

void Dijkstra::AddFloor(unsigned long floorId, unsigned int rows, unsigned int columns,
                        std::vector<Vertex *> *vertices, float costStraight, float costDiagonal) {
    std::vector<Vertex *> vector;
    int i, j, m, k;
    Vertex *from, *to;
    vector = *vertices;
    LOGD(TAG, "add floor: %dx%d=%d", rows, columns, vertices->size());
    for (i = 0; i < rows; i++) {
        for (j = 0; j < columns; j++) {
            from = vector[i * columns + j];
            if (from == nullptr) {
                continue;
            }
            for (k = i - 1; k <= i + 1; k++) {
                if (k < 0 || k >= rows)
                    continue;
                for (m = j - 1; m <= j + 1; m++) {
                    if (m < 0 || m >= columns)
                        continue;
                    if (m == j && k == i)
                        continue;
                    to = vector[k * columns + m];
                    if (to == nullptr) {
                        continue;
                    }
                    from->AddEdge(
                            new Edge(to, i - k == 0 || j - m == 0 ? costStraight : costDiagonal));
                }
            }
        }
    }
    LOGD(TAG, "edges added");
    floorsMap.insert(std::pair<unsigned long, std::vector<Vertex *>>(floorId, *vertices));
    floorsSizes.insert(std::pair<unsigned long, std::pair<unsigned int, unsigned int>>(floorId,
                                                                                       std::pair<unsigned int, unsigned int>(
                                                                                               rows,
                                                                                               columns)));
    LOGD(TAG, "floor sizes: %lu, %d, %d", floorId, floorsSizes.at(floorId).first,
         floorsSizes.at(floorId).second);
}


void Dijkstra::AddTransits(std::list<Transit *> transits, float costTransit) {
    std::vector<Vertex *> vertices;
    std::pair<unsigned int, unsigned int> pair;
    Vertex *vA, *vB;
    for (auto t : transits) {
        vertices = floorsMap.at(t->FloorA());
        pair = floorsSizes.at(t->FloorA());
        vA = vertices[t->RowA() * pair.second + t->ColumnA()];
        vertices = floorsMap.at(t->FloorB());
        pair = floorsSizes.at(t->FloorB());
        vB = vertices[t->RowB() * pair.second + t->ColumnB()];
        if (vA == nullptr || vB == nullptr) {
            LOGE(TAG, "transit empty: %d, %d", vA == nullptr, vB == nullptr);
            continue;
        }
        LOGD(TAG, "transit from [%d=%d, %d=%d] to [%d=%d, %d=%d]",
             t->RowA(), vA->Row(), t->ColumnA(), vA->Column(),
             t->RowB(), vB->Row(), t->ColumnB(), vB->Column()
        );
        vA->AddEdge(new Edge(vB, costTransit));
    }
}

std::list<unsigned int *> Dijkstra::FindPath(Transit *transit) {
    LOGD(TAG, "searching path between %lu, %d, %d and %lu, %d, %d",
         transit->FloorA(), transit->RowA(), transit->ColumnA(),
         transit->FloorB(), transit->RowB(), transit->ColumnB());
    LOGD(TAG, "map size: %d", floorsMap.size());
    for (auto it : floorsMap) {
        LOGD(TAG, "key: %lu", it.first);
    }

    std::list<unsigned int *> list;
    std::vector<Vertex *> startVertices, endVertices;
    auto it = floorsMap.find(transit->FloorA());
    if (it == floorsMap.end()) {
        LOGE(TAG, "start vertices are null");
        return list;
    }
    startVertices = (*it).second;
    LOGD(TAG, "2");
    it = floorsMap.find(transit->FloorB());
    if (it == floorsMap.end()) {
        LOGE(TAG, "end vertices are null");
        return list;
    }
    endVertices = (*it).second;
    LOGD(TAG, "3");
    std::pair<unsigned int, unsigned int> startSizes = floorsSizes.at(transit->FloorA());
    LOGD(TAG, "start size: %d, %d", startSizes.first, startSizes.second);
//    if (startSizes == nullptr) {
//        throw new std::invalid_argument("Sizes of Floor A are not initialized");
//    }
    std::pair<unsigned int, unsigned int> endSizes = floorsSizes.at(transit->FloorB());
    LOGD(TAG, "end size: %d, %d", endSizes.first, endSizes.second);
//    if (endSizes == nullptr) {
//        throw new std::invalid_argument("Sizes of Floor B are not initialized");
//    }
    Vertex *start, *end, *vertex;
    LOGD(TAG, "4");
    LOGD(TAG, "trying to get %d, %d: %d with length %d", transit->RowA(), transit->ColumnA(),
         transit->RowA() * startSizes.second + transit->ColumnA(), startVertices.size());
    start = startVertices[transit->RowA() * startSizes.second + transit->ColumnA()];
    LOGD(TAG, "trying to get %d, %d: %d with length %d", transit->RowB(), transit->ColumnB(),
         transit->RowB() * endSizes.second + transit->ColumnB(), endVertices.size());
    end = endVertices[transit->RowB() * endSizes.second + transit->ColumnB()];

    if (start == nullptr) {
        LOGE(TAG, "start vertex is null");
        return list;
    }
    if (end == nullptr) {
        LOGE(TAG, "end vertex is null");
        return list;
    }
    LOGD(TAG, "clear distances");
    ClearDistances();
    LOGD(TAG, "calculate distances");
    CalculateDistances(start);
    start->SetDistance(-1);
    end->SetDistance(0);
    vertex = end;
    LOGD(TAG, "starting loop");
    while (vertex != nullptr) {
        list.push_back(new unsigned int[3]{vertex->Row(), vertex->Column(), vertex->Floor()});
        if (vertex == start) {
            break;
        }
        vertex = vertex->Previous();
    }
    list.reverse();
    LOGD(TAG, "path length: %d", list.size());
    return list;
}

void Dijkstra::CalculateDistances(Vertex *start) {
    float distance;
    std::priority_queue<Vertex *, std::vector<Vertex *>, VertexComparator> queue;
    std::vector<Edge *>::iterator it;
    start->SetDistance(0);
    queue.push(start);
    int i, size;
    while (!queue.empty()) {
        Vertex *vertex = queue.top();
        queue.pop();
        i = 0;
        size = vertex->Edges()->size();
        for (it = vertex->Edges()->begin(); it != vertex->Edges()->end(); it++, i++) {
            Edge *edge = *it;
            if (edge->VertexTo() == nullptr) {
                LOGE(TAG, "edge with empty vertex: %d of %d", i, size);
                continue;
            }
            if (i >= size) {
                LOGW(TAG, "reached maximum size");
                break;
            }
            if (edge->VertexTo()->IsVisited())
                continue;
            distance = edge->Cost() + vertex->Distance();
            if (distance < edge->VertexTo()->Distance()) {
                edge->VertexTo()->SetPrevious(vertex);
                edge->VertexTo()->SetDistance(distance);
                queue.push(edge->VertexTo());
            }
        }
        vertex->SetVisited(true);
    }
}

void Dijkstra::ClearDistances() {
    std::vector<Vertex *>::iterator iterator;
    Vertex *vertex;
    for (auto pair : floorsMap) {
        std::vector<Vertex *> vector = pair.second;
        for (iterator = vector.begin(); iterator != vector.end(); iterator++) {
            vertex = *iterator;
            if (vertex != nullptr) {
                vertex->SetDistance(FLT_MAX);
                vertex->SetPrevious(nullptr);
            }
        }
    }
}