import java.io.*;
import java.math.*;
import java.text.*;
import java.util.*;
import java.util.regex.*;

public class Solution {
    private static class Node {
        final int index;
        final Set<Node> neighbors = new HashSet<>();

        Node(int index) {
            this.index = index;
        }
    }

    /*
     * Complete the journeyScheduling function below.
     */
    static long[] journeyScheduling(int n, int[][] roads, int[][] journeys) {
        Node[] nodes = new Node[n];
        for (int i = 0; i < n; i++) {
            nodes[i] = new Node(i);
        }
        for (int[] road : roads) {
            nodes[road[0] - 1].neighbors.add(nodes[road[1] - 1]);
            nodes[road[1] - 1].neighbors.add(nodes[road[0] - 1]);
        }

        Node start = findEnd(nodes[0]);
        Node end = findEnd(start);
        List<Node> diameterPath = findPath(start, end);
        int diameter = diameterPath.size() - 1;
        int[] distances = new int[n];
        if ((diameter & 1) == 0) {
            maxDistances(diameterPath.get(diameter/2), null, diameter/2, distances);
        } else {
            maxDistances(diameterPath.get(diameter/2), diameterPath.get(diameter/2 + 1), diameter/2 + 1, distances);
            maxDistances(diameterPath.get(diameter/2 + 1), diameterPath.get(diameter/2), diameter/2 + 1, distances);
        }
//        System.out.println(String.format("Diameter: %d, distances: %s", diameter, Arrays.toString(distances)));

        long[] results = new long[journeys.length];
        for (int i = 0; i < journeys.length; i++) {
            results[i] = distances[journeys[i][0] - 1] + ((long) diameter)*(journeys[i][1] - 1);
        }
        return results;
    }

    private static class State {
        final Node cur;
        final Node prev;
        final Iterator<Node> children;
        final int distance;

        State(Node cur, Node prev, int distance) {
            this.cur = cur;
            this.prev = prev;
            this.children = cur.neighbors.iterator();
            this.distance = distance;
        }
    }

    private static Node findEnd(Node cur) {
        Node end = cur;
        int maxDistance = -1;
        Deque<State> stack = new ArrayDeque<>();
        stack.addFirst(new State(cur, null, 0));
        while (!stack.isEmpty()) {
            State state = stack.peekFirst();
            if (state.children.hasNext()) {
                Node child = state.children.next();
                if (child == state.prev) {
                    // Do nothing
                } else if (child.neighbors.size() == 1) {
                    if (state.distance > maxDistance) {
                        maxDistance = state.distance;
                        end = child;
                    }
                } else {
                    stack.addFirst(new State(child, state.cur, state.distance + 1));
                }
            } else {
                stack.removeFirst();
            }
        }
        return end;
    }

    private static List<Node> findPath(Node cur, Node goal) {
        Deque<State> stack = new ArrayDeque<>();
        stack.addFirst(new State(cur, null, 0));
        while (stack.peekFirst().cur != goal) {
            State state = stack.peekFirst();
            if (state.children.hasNext()) {
                Node child = state.children.next();
                if (child != state.prev) {
                    stack.addFirst(new State(child, state.cur, 0));
                }
            } else {
                stack.removeFirst();
            }
        }

        List<Node> path = new ArrayList<>();
        while (!stack.isEmpty()) {
            path.add(stack.removeFirst().cur);
        }
        return path;
    }

    private static void maxDistances(Node cur, Node prev, int distance, int[] distances) {
        distances[cur.index] = distance;
        Deque<State> stack = new ArrayDeque<>();
        stack.addFirst(new State(cur, prev, distance));
        while (!stack.isEmpty()) {
            State state = stack.peekFirst();
            if (state.children.hasNext()) {
                Node child = state.children.next();
                if (child != state.prev) {
                    distances[child.index] = state.distance + 1;
                    stack.addFirst(new State(child, state.cur, state.distance + 1));
                }
            } else {
                stack.removeFirst();
            }
        }
    }

    private static final Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) throws IOException {
        BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(System.getenv("OUTPUT_PATH")));

        String[] nm = scanner.nextLine().split(" ");
        scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])*");

        int n = Integer.parseInt(nm[0]);

        int m = Integer.parseInt(nm[1]);

        int[][] roads = new int[n-1][2];

        for (int roadsRowItr = 0; roadsRowItr < n-1; roadsRowItr++) {
            String[] roadsRowItems = scanner.nextLine().split(" ");
            scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])*");

            for (int roadsColumnItr = 0; roadsColumnItr < 2; roadsColumnItr++) {
                int roadsItem = Integer.parseInt(roadsRowItems[roadsColumnItr]);
                roads[roadsRowItr][roadsColumnItr] = roadsItem;
            }
        }

        int[][] journeys = new int[m][2];

        for (int journeysRowItr = 0; journeysRowItr < m; journeysRowItr++) {
            String[] journeysRowItems = scanner.nextLine().split(" ");
            scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])*");

            for (int journeysColumnItr = 0; journeysColumnItr < 2; journeysColumnItr++) {
                int journeysItem = Integer.parseInt(journeysRowItems[journeysColumnItr]);
                journeys[journeysRowItr][journeysColumnItr] = journeysItem;
            }
        }

        long[] result;
//        try {
            result = journeyScheduling(n, roads, journeys);
/*        } catch (StackOverflowError e) {
            result = new long[1];
        }*/

        for (int resultItr = 0; resultItr < result.length; resultItr++) {
            bufferedWriter.write(String.valueOf(result[resultItr]));

            if (resultItr != result.length - 1) {
                bufferedWriter.write("\n");
            }
        }

        bufferedWriter.newLine();

        bufferedWriter.close();

        scanner.close();
    }
}
