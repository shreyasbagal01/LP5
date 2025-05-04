#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <omp.h>

using namespace std;

class Graph {
public:
    int V;  // Number of vertices
    vector<vector<int>> adj;  // Adjacency list

    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);  // Since the graph is undirected
    }

    // Parallel BFS using OpenMP
    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        #pragma omp parallel
        {
            #pragma omp single
            {
                visited[start] = true;
                q.push(start);
            }
            
            while (!q.empty()) {
                int v;

                #pragma omp critical
                {
                    if (!q.empty()) {
                        v = q.front();
                        q.pop();
                        cout << v << " ";
                    }
                }

                #pragma omp parallel for
                for (int i = 0; i < adj[v].size(); i++) {
                    int u = adj[v][i];
                    if (!visited[u]) {
                        visited[u] = true;
                        #pragma omp critical
                        q.push(u);
                    }
                }
            }
        }
    }

    // Parallel DFS using OpenMP
    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        stack<int> s;

        #pragma omp parallel
        {
            #pragma omp single
            {
                s.push(start);
                visited[start] = true;
            }

            while (!s.empty()) {
                int v;

                #pragma omp critical
                {
                    if (!s.empty()) {
                        v = s.top();
                        s.pop();
                        cout << v << " ";
                    }
                }

                #pragma omp parallel for
                for (int i = 0; i < adj[v].size(); i++) {
                    int u = adj[v][i];
                    if (!visited[u]) {
                        visited[u] = true;
                        #pragma omp critical
                        s.push(u);
                    }
                }
            }
        }
    }
};

int main() {
    Graph g(7);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);

    cout << "Parallel BFS starting from vertex 0: " << endl;
    g.parallelBFS(0);

    cout << "\nParallel DFS starting from vertex 0: " << endl;
    g.parallelDFS(0);

    return 0;
}
