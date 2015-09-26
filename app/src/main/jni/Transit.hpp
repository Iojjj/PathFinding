//
// Created by Alexander Vlasov on 26.09.2015.
//

#ifndef PATHFINDING_TRANSITION_H
#define PATHFINDING_TRANSITION_H


class Transit {
    unsigned long floorA, floorB;
    unsigned int rowA, rowB, columnA, columnB;

public:
    inline Transit(unsigned long floorA, unsigned int rowA, unsigned int columnA,
               unsigned long floorB, unsigned int rowB, unsigned int columnB) {
        this->floorA = floorA;
        this->floorB = floorB;
        this->rowA = rowA;
        this->rowB = rowB;
        this->columnA = columnA;
        this->columnB = columnB;
    };
    inline unsigned long FloorA() { return floorA; }
    inline unsigned long FloorB() { return floorB; }
    inline unsigned int RowA() { return rowA; }
    inline unsigned int RowB() { return rowB; }
    inline unsigned int ColumnA() { return columnA; }
    inline unsigned int ColumnB() { return columnB; }
};


#endif //PATHFINDING_TRANSITION_H
