package test.pathfinding;

import android.graphics.Point;
import android.support.annotation.NonNull;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;

/**
 * Created by Alexander Vlasov on 22.08.2015.
 */
public class DijkstraOld implements IAlgorithm {

    static {
        System.loadLibrary("test-boost");
    }

    private int rows, columns;
    private final List<Vertex> vertices = new ArrayList<>();
    private boolean inited;
    private long object;
    private final Comparator<Edge> edgeComparator = new Comparator<Edge>() {
        @Override
        public int compare(Edge lhs, Edge rhs) {
            return Double.compare(lhs.cost, rhs.cost);
        }
    };

    private final Comparator<Vertex> vertexComparator = new Comparator<Vertex>() {
        @Override
        public int compare(Vertex lhs, Vertex rhs) {
            return Double.compare(lhs.distanceTag, rhs.distanceTag);
        }
    };
    public void init(@NonNull byte[][] map, float costStraight, float costDiagonal) {
        vertices.clear();
        int i, j, k, m;
        rows = map.length;
        columns = map[0].length;
        for (i =0; i<rows; i++) {
            for (j=0; j<columns; j++) {
                if (map[i][j] == PATH)
                    vertices.add(new Vertex(i, j));
                else
                    vertices.add(null);
            }
        }
        Vertex from, to;
        for (i =0; i<rows; i++) {
            for (j = 0; j <columns; j++) {
                from = vertices.get(i * columns + j);
                if (from == null)
                    continue;
                for (k = i-1; k<=i+1; k++) {
                    if (k < 0 || k >= rows)
                        continue;
                    for (m = j-1; m<=j+1; m++) {
                        if (m < 0 || m >= columns)
                            continue;
                        if (m == j && k == i)
                            continue;
                        to = vertices.get(k * columns + m);
                        if (to == null)
                            continue;
                        if (i - k == 0 || j - m == 0) {
                            //    0
                            // 0  x  0
                            //    0
                            from.edges.add(new Edge(to, costStraight));
                        } else {
                            // 0     0
                            //    x
                            // 0     0
                            from.edges.add(new Edge(to, costDiagonal));
                        }
                    }
                }
                Collections.sort(from.edges, edgeComparator);
            }
        }
        inited = true;
    }


    public int[][] calculateRoute(int startRow, int startCol, int endRow, int endCol) {
        if (!inited)
            throw new IllegalStateException("You must init algorithm first");
        for (Vertex vertex : vertices) {
            if (vertex == null)
                continue;
            vertex.distanceTag = Float.POSITIVE_INFINITY;
            vertex.prev = null;
        }
        Vertex start = vertices.get(startRow * columns + startCol);
        Vertex end = vertices.get(endRow * columns + endCol);
        if (start == null)
            throw new IllegalArgumentException("Start vertex is null");
        if (end == null)
            throw new IllegalArgumentException("End vertex is null");
        calculateDistances(start);
        return getPath(start, end).toArray();
    }

    private Route getPath(Vertex start, Vertex end) {
        start.distanceTag = -1;
        end.distanceTag = 0;
        Route route = new Route();
        fillRoute(route, end);
        Collections.reverse(route.getPoints());
        return route;
    }

    private void fillRoute(Route route, Vertex vertex) {
        if (vertex == null)
            return;
        route.addPoint(vertex.location);
        if (vertex.prev != null && Double.compare(vertex.prev.distanceTag, 0) == 0) {
            route.addPoint(vertex.prev.location);
            return;
        }
        fillRoute(route, vertex.prev);
    }

    private void calculateDistances(Vertex start) {
        start.distanceTag = 0;
        PriorityQueue<Vertex> queue = new PriorityQueue<>(10, vertexComparator);
        queue.add(start);
        float distance;
        while(!queue.isEmpty()) {
            Vertex vertex = queue.poll();
            for (Edge edge : vertex.edges) {
                if (edge.vertex.visited)
                    continue;
                distance = vertex.distanceTag + edge.cost;
                if (distance < edge.vertex.distanceTag) {
                    edge.vertex.prev = vertex;
                    edge.vertex.distanceTag = distance;
                    queue.add(edge.vertex);
                }
            }
            vertex.visited = true;
        }
    }

    @Override
    public void init(byte[][][] maps, float costStraight, float costDiagonal, float costTransit) {

    }

    @Override
    public int[][] calculateRoute(int startRow, int startCol, int startFloor, int endRow, int endCol, int endFloor) {
        return new int[0][];
    }

    private static class Vertex {

        private final List<Edge> edges = new ArrayList<>();
        private final Point location;
        private Vertex prev;
        private boolean visited;
        private float distanceTag = Float.POSITIVE_INFINITY;

        public Vertex(int x, int y) {
            location = new Point(x, y);
        }
    }

    private static class Edge {
        private Vertex vertex;
        private float cost;

        public Edge(Vertex vertex, float cost) {
            this.vertex = vertex;
            this.cost = cost;
        }
    }
}
