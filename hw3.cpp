#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <queue>

using namespace std;

// BFS
bool bfs(vector<vector<int> > &residual_graph, int s, int t, vector<int> &parent) {
    int size = residual_graph.size();
    bool visited[size];
    memset(visited, 0, sizeof(visited));
 
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
 
    // Standard BFS Loop
    while (!q.empty()) {
        int u = q.front();
        q.pop();
 
        for (int v = 0; v < size; v++) {
            if (visited[v] == false && residual_graph[u][v] > 0) {
                if (v == t) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}



// Ford-Fulkerson algorithm
int ford_fulkerson(vector<vector<int> > &graph, int source, int sink)
{
    int u, v;
    int size = graph.size();
 
    // residual graph
    vector<vector<int> > residual_graph;
    for (int i = 0; i < size; i++) {
        vector<int> temp;
        for (int j = 0; j < size; j++) {
            temp.push_back(graph[i][j]);
        }
        residual_graph.push_back(temp);
    }

    vector<int> parent(size+1); // This array is filled by BFS and to
                   // store path
 
    int max_flow = 0; // There is no flow initially
 
    // Augment the flow while there is path from source to
    // sink
    while (bfs(residual_graph, source, sink, parent)) {
        int path_flow = INT_MAX;
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            path_flow = min(path_flow, residual_graph[u][v]);
        }
 
        // update residual capacities of the edges and
        // reverse edges along the path
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            residual_graph[u][v] -= path_flow;
            residual_graph[v][u] += path_flow;
        }
 
        // Add path flow to overall flow
        max_flow += path_flow;
    }
 
    // Return the overall flow
    return max_flow;
}

int main()
{

    // graph
    vector<vector<int> > graph;
    vector<vector<float> > graph_probability;

    // read from file
    ifstream file;
    file.open("input.txt");
    string line;
    int i = 0;
    getline(file, line);
    int n = stoi(line);
    for (int i = 0; i < n; i++)
    {
        vector<int> temp;
        // read from file and store in graph
        getline(file, line);
        stringstream ss(line);
        int j = 0;
        while (ss >> line)
        {
            temp.push_back(stoi(line));
            j++;
        }
        graph.push_back(temp);
    }

    // read from file
    getline(file, line);
    for (int i = 0; i < n; i++)
    {
        vector<float> temp;
        // read from file and store in graph
        getline(file, line);
        stringstream ss(line);
        int j = 0;
        while (ss >> line)
        {
            temp.push_back(stof(line));
            j++;
        }
        graph_probability.push_back(temp);
    }

    file.close();
    // print graph
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }

    // print graph_probability
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << graph_probability[i][j] << " ";
        }
        cout << endl;
    }

    // Ford-Fulkerson algorithm
    int max_flow = ford_fulkerson(graph,0, n - 1);
    cout << "Max flow: " << max_flow << endl;
}