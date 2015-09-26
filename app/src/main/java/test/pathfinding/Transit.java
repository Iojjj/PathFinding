package test.pathfinding;

/**
 * Created by Alexander Vlasov on 26.09.2015.
 */
public class Transit {
    private long fromFloor, toFloor;
    private int fromRow,  fromCol, toRow, toCol;

    public Transit(long fromFloor, int fromRow, int fromCol, long toFloor, int toRow, int toCol) {
        this.fromFloor = fromFloor;
        this.toFloor = toFloor;
        this.fromRow = fromRow;
        this.fromCol = fromCol;
        this.toRow = toRow;
        this.toCol = toCol;
    }

    public long getFromFloor() {
        return fromFloor;
    }

    public long getToFloor() {
        return toFloor;
    }

    public int getFromRow() {
        return fromRow;
    }

    public int getFromCol() {
        return fromCol;
    }

    public int getToRow() {
        return toRow;
    }

    public int getToCol() {
        return toCol;
    }
}
