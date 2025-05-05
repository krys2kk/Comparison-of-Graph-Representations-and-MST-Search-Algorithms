Graph Algorithm Benchmark
This project benchmarks three graph algorithms — Dijkstra, Bellman-Ford, and BFS — using two graph representations: adjacency matrix and adjacency list.

🧪 Algorithms
Dijkstra: Fast for non-negative weights

Bellman-Ford: Supports negative weights

BFS: Efficient for unweighted graphs

🔧 How It Works
Graphs are randomly generated with various sizes (10–500) and densities (0.25–1.0).

Each algorithm runs from a random starting node.

Average execution time is measured and saved to graphs_results22.csv.

🛠️ Build & Run
bash
g++ main.cpp -std=c++17 -o graph_benchmark
./graph_benchmark
📄 Output Format (CSV)
size;density;dijkstra_ms;bellman_ford_ms;bfs_ms
💡 Notes
Dijkstra and BFS are fast on sparse graphs.

Bellman-Ford is slower, especially on dense graphs.
