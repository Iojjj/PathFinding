package test.pathfinding;

import android.graphics.Point;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Alexander Vlasov on 22.08.2015.
 */
public class Route {
    private final List<Point> points = new ArrayList<>();

    public void addPoint(Point point) {
        points.add(point);
    }

    public List<Point> getPoints() {
        return points;
    }

    public int[][] toArray() {
        int[][] answer = new int[points.size()][];
        int i = 0;
        for (Point point : points)
            answer[i++] = new int[] { point.x, point.y };
        return answer;
    }
}
