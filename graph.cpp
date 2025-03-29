#include "graph.h"
#include <QSet>


void Graph::addNode(int id, QPointF pos) {
    nodes[id] = new Node(id, pos);
}


void Graph::addEdge(int start, int end, double weight) {
    edges.push_back(Edge(start, end, weight));
}

QVector<int> Graph::dijkstra(int start, int end) {
    QMap<int, double> dist;
    QMap<int, int> prev;
    QSet<int> visited;

    for (int node : nodes.keys()) {
        dist[node] = std::numeric_limits<double>::infinity();
    }
    dist[start] = 0;

    while (!visited.contains(end)) {
        int u = -1;
        for (int node : nodes.keys()) {
            if (!visited.contains(node) && (u == -1 || dist[node] < dist[u])) {
                u = node;
            }
        }

        if (dist[u] == std::numeric_limits<double>::infinity())
            break;

        visited.insert(u);

        for (Edge e : edges) {
            if (e.startNode == u) {
                int v = e.endNode;
                double alt = dist[u] + e.weight;
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                }
            }
        }
    }

    QVector<int> path;
    for (int at = end; at != start; at = prev[at]) {
        path.prepend(at);
    }
    path.prepend(start);
    return path;
}
