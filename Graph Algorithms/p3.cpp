#include <bits/stdc++.h>
#include <iostream>
using namespace std;
#define INF 999999

void floydWarshall(int graph[501][1001], int n) {
    int dist[501][1001], i, j, k;
    for (i = 1; i <= n; i++)
        for (j = 1; j <= n; j++)
            dist[i][j] = graph[i][j];

    for (k = 1; k <= n; k++) {
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= n; j++) {
                if (dist[i][j] > (dist[i][k] + dist[k][j]) &&
                    (dist[k][j] != INF && dist[i][k] != INF))
                        dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }

    int min = INF;
    for (int i = 1; i <= n; i++) {
        if (dist[i][i] <= min) {
            min = dist[i][i];
        }
    }

    ofstream fout("p3.out");
    fout << min;
    fout.close();
}

int main() {
    int n, m;
    ifstream fin("p3.in");
    fin >> n >> m;
    int graph[501][1001];

    int u, v, w;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            graph[i][j] = INF;
        }
    }

    for (int i = 1; i <= m; i++) {
        fin >> u >> v >> w;
        graph[u][v] = w;
    }

    fin.close();
    floydWarshall(graph, n);
    return 0;
}
