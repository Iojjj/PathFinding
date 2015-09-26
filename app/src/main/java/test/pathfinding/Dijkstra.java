package test.pathfinding;

import android.support.annotation.NonNull;

/**
 * Created by Alexander Vlasov on 22.08.2015.
 */
public class Dijkstra implements IAlgorithm {

    static {
        System.loadLibrary("test-boost");
    }

    private long object;

    private native long newObject();

    private native void deleteObject(long object);

    private native void addFloor(long object, long floorId, byte[][] waypoints, float costStraight, float costDiagonal);

    private native void createTransits(long object, Transit[] transits, float costTransit);

    private native int[][] findPath(long object, long fromFloor, int fromRow, int fromCol, long toFloor, int toRow, int toCol);

    public Dijkstra() {
        object = newObject();
    }

    public void release() {
        deleteObject(object);
    }

    public void addFloor(long floorId, @NonNull byte[][] map, float costStraight, float costDiagonal) {
        addFloor(object, floorId, map, costStraight, costDiagonal);
    }

    public void createTransits(Transit[] transits, float costTransit) {
        createTransits(object, transits, costTransit);
    }

    public int[][] findPath(long fromFloor, int fromRow, int fromCol, long toFloor, int toRow, int toCol) {
        return findPath(object, fromFloor, fromRow, fromCol, toFloor, toRow, toCol);
    }

    @Override
    public void init(byte[][][] maps, float costStraight, float costDiagonal, float costTransit) {
        object = newObject();
    }

    @Override
    public int[][] calculateRoute(int startRow, int startCol, int startFloor, int endRow, int endCol, int endFloor) {
//        return getPath(object, startRow, startCol, startFloor, endRow, endCol, endFloor);
        return null;
    }
}
