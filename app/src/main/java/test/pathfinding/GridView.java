package test.pathfinding;

import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Build;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by Alexander Vlasov on 23.08.2015.
 */
public class GridView extends View {

    private static final int size = 80;
    private static final int radius = 30;
    private byte[][] matrix;
    private int[][] route;
    private Paint pathPint, obstaclePaint, borderPaint;

    public GridView(Context context) {
        super(context);
        init();
    }

    public GridView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public GridView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public GridView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    private void init() {
        pathPint = new Paint();
        pathPint.setColor(Color.GREEN);
        pathPint.setStyle(Paint.Style.FILL);
        obstaclePaint = new Paint();
        obstaclePaint.setColor(Color.RED);
        obstaclePaint.setStyle(Paint.Style.FILL);
        borderPaint = new Paint();
        borderPaint.setColor(Color.BLACK);
        borderPaint.setStyle(Paint.Style.STROKE);
    }

    public GridView setMatrix(byte[][] matrix) {
        this.matrix = matrix;
        return this;
    }

    public GridView setRoute(int[][] route) {
        this.route = route;
        return this;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        float size = 0, radius = 0;
        if (matrix != null) {
            size = 1f * Math.min(canvas.getWidth(), canvas.getHeight()) / Math.min(matrix.length, matrix[0].length);
            radius = size;
            for (int i=0; i<matrix.length; i++) {
                for (int j=0; j<matrix[i].length; j++) {
                    if (matrix[i][j] == IAlgorithm.OBSTACLE)
                        canvas.drawRect(j * size, i * size, (j + 1) * size, (i+1) * size, obstaclePaint);
                    canvas.drawRect(j * size, i * size, (j + 1) * size, (i+1) * size, borderPaint);
                }
            }
        }
        if (route != null) {
            for (int[] point : route) {
                canvas.drawCircle((point[1] + .5f) * size, (point[0] + .5f) * size, radius, pathPint);
            }
        }
    }
}
