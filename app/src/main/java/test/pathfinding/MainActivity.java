package test.pathfinding;

import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Random;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {

    private IAlgorithm algorithm = new DijkstraOld();
    private TextView text;
    private ListView log;
    private AsyncTask task;
    private GridView gridView;
    private ArrayAdapter<String> adapter;
    private boolean draw = false;
    private final Random random = new Random();
    private final byte[][] matrix1 = new byte[][]{
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        gridView = (GridView) findViewById(R.id.gridView);
        text = (TextView) findViewById(R.id.text);
        log = (ListView) findViewById(R.id.log);
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        log.setAdapter(adapter);
        getSupportActionBar().setSubtitle("Java (Not Draw)");
        run();
    }

    private void run() {
        if (task != null)
            task.cancel(true);
        task = new AsyncTask<Void, Long, int[][]>() {

            byte[][] matrix;

            @Override
            protected void onPreExecute() {
                super.onPreExecute();
                text.setText("Initializing");
            }

            @Override
            protected int[][] doInBackground(Void... params) {
                final int width = 500;
                final int height = 500;
                matrix = new byte[height][width];
                for (int i=0; i<height; i++) {
                    for (int j=0; j<width; j++)
                        matrix[i][j] = (byte) (i== 0 && j == 0 || i == height - 1 && j == width - 1 || random.nextDouble() > 0.3 ? 0 : 1);
                }
                Dijkstra dijkstra = new Dijkstra();
                long nano = System.nanoTime();
                dijkstra.addFloor(1, matrix, 1, (float) Math.sqrt(2));
                dijkstra.addFloor(2, matrix, 1, (float) Math.sqrt(2));
                dijkstra.addFloor(3, matrix, 1, (float) Math.sqrt(2));
                dijkstra.addFloor(4, matrix, 1, (float) Math.sqrt(2));
                dijkstra.createTransits(
                        new Transit[]{
                                new Transit(1, 499, 499, 2, 0, 0),
                                new Transit(2, 499, 499, 3, 0, 0),
                                new Transit(3, 499, 499, 4, 0, 0),
                                new Transit(1, 499, 499, 4, 0, 0)
                        }, 5);
                long diff = System.nanoTime() - nano;
                publishProgress(diff);
                nano = System.nanoTime();
                int[][] route = dijkstra.findPath(1, 0, 0, 4, matrix.length - 1, matrix[0].length - 1);//algorithm.calculateRoute(0, 0, matrix.length - 1, matrix[0].length - 1);
                diff = System.nanoTime() - nano;
                publishProgress(diff);
                return route;
            }

            @Override
            protected void onProgressUpdate(Long... values) {
                super.onProgressUpdate(values);
                text.append(String.format("\r\n%.3f", TimeUnit.MILLISECONDS.convert(values[0], TimeUnit.NANOSECONDS) / 1000f));
            }

            @Override
            protected void onPostExecute(int[][] route) {
                super.onPostExecute(route);
                if (route == null || route.length <= 1) {
                    Toast.makeText(MainActivity.this, "Path not found", Toast.LENGTH_SHORT).show();
                    return;
                }
                adapter.setNotifyOnChange(false);
                for (int[] cell : route) {
                    adapter.add(String.format("[%d, %d, %d]", cell[2], cell[0], cell[1]));
                }
                adapter.notifyDataSetChanged();
                if (draw) {
                    gridView.setMatrix(matrix);
                    gridView.setRoute(route);
                    text.append("\r\nDrawing...");
                } else {
                    gridView.setMatrix(null)
                            .setRoute(null);
                }
                gridView.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        gridView.invalidate();
                    }
                }, 3000);
            }
        }.execute();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        gridView.setMatrix(null).setRoute(null).invalidate();
        if (id == R.id.action_settings) {
            if (algorithm instanceof DijkstraOld) {
                algorithm = new Dijkstra();
                getSupportActionBar().setSubtitle("Native" + (draw ? " (Draw)" : " (Not Draw)"));
            } else if (algorithm instanceof Dijkstra) {
                ((Dijkstra) algorithm).release();
                algorithm = new DijkstraOld();
                getSupportActionBar().setSubtitle("Java" + (draw ? " (Draw)" : " (Not Draw)"));
            }
            run();
            return true;
        } else if (id == R.id.action_draw) {
            draw = !draw;
            getSupportActionBar().setSubtitle((algorithm instanceof DijkstraOld ? "Java" : "Native") + (draw ? " (Draw)" : " (Not Draw)"));
            if (algorithm instanceof Dijkstra)
                ((Dijkstra) algorithm).release();
            run();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
