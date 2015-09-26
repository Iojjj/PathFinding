package test.pathfinding;

/**
 * Created by Alexander Vlasov on 22.08.2015.
 */
public interface IAlgorithm {

    int PATH = 0;
    int OBSTACLE = 1;

    void init(byte[][][] maps, float costStraight, float costDiagonal, float costTransit);
    int[][] calculateRoute(int startRow, int startCol, int startFloor, int endRow, int endCol, int endFloor);
}
