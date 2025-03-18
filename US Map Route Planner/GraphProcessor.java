import java.security.InvalidAlgorithmParameterException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Scanner;
import java.util.Set;
import java.util.Stack;
import java.io.FileInputStream;

/**
 * Models a weighted graph of latitude-longitude points
 * and supports various distance and routing operations.
 * To do: Add your name(s) as additional authors
 * @author Kavishka Bartlett
 * @author Brandon Fain
 *
 */
public class GraphProcessor {
    private Map<Point, Set<Point>> neighbors;
    private Map<String, Point> list;
    private Comparator<Point> comp;
    /**
     *
     * Creates and initializes a graph from a source data
     * file in the .graph format. Should be called
     * before any other methods work.
     * @param file a FileInputStream of the .graph file
     * @throws Exception if file not found or error reading
     */
    public void initialize(FileInputStream file) throws Exception {
        Map<Integer, Point> location = new HashMap<>();
        neighbors = new HashMap<>();
        list = new HashMap<>();
        Scanner scan = new Scanner(file);
        int points = scan.nextInt();
        int edges = scan.nextInt();
        scan.nextLine();
        for (int i = 0; i < points; i++ ){
            String[] vertexInfo = scan.nextLine().split(" "); // read next line, split by spaces
            list.put(vertexInfo[0], new Point((Double.parseDouble(vertexInfo[1])),(Double.parseDouble(vertexInfo[2]))));                                  // add name with associated point to list
            neighbors.put(new Point((Double.parseDouble(vertexInfo[1])),(Double.parseDouble(vertexInfo[2]))), new HashSet<Point>());                   // add point with empty adjacency list
            location.put(i, new Point((Double.parseDouble(vertexInfo[1])),(Double.parseDouble(vertexInfo[2]))));                                      // just for the next step
        }
        for (int i = 0; i < edges; i++) {
            String[] edgeData = scan.nextLine().split(" ");
            neighbors.get(location.get(Integer.parseInt(edgeData[0]))).add(location.get(Integer.parseInt(edgeData[1])));    // add the pair connection to the adjacency list
            neighbors.get(location.get(Integer.parseInt(edgeData[1]))).add(location.get(Integer.parseInt(edgeData[0])));    // add the pair connection to the adjacency list (undirect so both ways)
        }
        scan.close();
    }

    /**
     * Searches for the point in the graph that is closest in
     * straight-line distance to the parameter point p
     * @param p A point, not necessarily in the graph
     * @return The closest point in the graph to p
     */
    public Point nearestPoint(Point p) {
        Point answer = null;
        double min = -1;
        for (Point temp : neighbors.keySet()) {
            if (min == -1) {
                min = p.distance(temp);
                answer = temp; 
            }
            double distance = p.distance(temp);
            if (distance < min) {
                min = distance;
                answer = temp;
            }
        }
        return answer;
    }


    /**
     * Calculates the total distance along the route, summing
     * the distance between the first and the second Points, 
     * the second and the third, ..., the second to last and
     * the last. Distance returned in miles.
     * @param start Beginning point. May or may not be in the graph.
     * @param end Destination point May or may not be in the graph.
     * @return The distance to get from start to end
     */
    public double routeDistance(List<Point> route) {
        double initialDist = 0.0;
        for (int i=1; i<route.size(); i++) {
            Point temp1 = route.get(i-1);
            Point temp2 = route.get(i);
            initialDist += temp1.distance(temp2);
        }
        return initialDist;
    }
    

    /**
     * Checks if input points are part of a connected component
     * in the graph, that is, can one get from one to the other
     * only traversing edges in the graph
     * @param p1 one point
     * @param p2 another point
     * @return true if p2 is reachable from p1 (and vice versa)
     */
    public boolean connected(Point p1, Point p2) {
        //if (!(neighbors.containsKey(p1)) || !(neighbors.containsKey(p2)))
          //  return false;
        HashSet<Point> visited = new HashSet<>();
        HashMap<Point, Point> previous = new HashMap<>();
        Stack<Point> toExplore = new Stack<>();
        toExplore.add(p1);
        Point current;
        while(!toExplore.isEmpty()) {
            current = toExplore.pop();
            for (Point temp : neighbors.get(current)) {
                if (!(visited.contains(temp))) {
                    previous.put(temp, current);
                    visited.add(temp);
                    toExplore.add(temp);
                }
            }
        }
        if (visited.contains(p2))
            return true;
        else 
            return false;
    }


    /**
     * Returns the shortest path, traversing the graph, that begins at start
     * and terminates at end, including start and end as the first and last
     * points in the returned list. If there is no such route, either because
     * start is not connected to end or because start equals end, throws an
     * exception.
     * @param start Beginning point.
     * @param end Destination point.
     * @return The shortest path [start, ..., end].
     * @throws InvalidAlgorithmParameterException if there is no such route, 
     * either because start is not connected to end or because start equals end.
     */
    public List<Point> route(Point start, Point end) throws InvalidAlgorithmParameterException {
        //check if connected first
        if (start.equals(end) || !(connected(start, end))) 
            throw new InvalidAlgorithmParameterException("No path between start and end");
        HashMap<Point, Point> previous = new HashMap<>();
        HashMap<Point, Double> distance = new HashMap<>();
        comp = (a, b) -> Double.compare(distance.get(a), distance.get(b));
        PriorityQueue<Point> toExplore = new PriorityQueue<>(comp);
        Point current = start;
        distance.put(current, 0.0);
        toExplore.add(current);
        while(!(toExplore.isEmpty())) {
            current = toExplore.remove();
            for (Point temp: neighbors.get(current)) {
                Double weight = current.distance(temp);
                if(!distance.containsKey(temp) || distance.get(temp) > distance.get(current) + weight) {
                    distance.put(temp, distance.get(current) + weight);
                    previous.put(temp, current);
                    toExplore.add(temp);
                }
            }
        }
        ArrayList<Point> route = new ArrayList<>();
        route.add(end);
        Point temp = previous.get(end); //Find the route by going back to front
        Point last = temp; 
        while (!last.equals(start)) {
            route.add(last);
            last = previous.get(last);
        }
        route.add(start);
        ArrayList<Point> answer = new ArrayList<>();
        for (int i=route.size()-1; i>=0; i--) {
            answer.add(route.get(i));
        }
        return answer;
    }
}
