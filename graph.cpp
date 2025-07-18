#include "graph.h"
#include "trafficlight.h"

#include <QSet>
#include <limits>

void Graph::addNode(int id, QPointF pos, NodeType type, RoadType roadtype) {
    Node* node = new Node(id, pos, type, roadtype);
    nodes[id] = node;

    if (type == NodeType::Spawn) {
        spawnNodes.append(node);
    } else if (type == NodeType::Despawn) {
        despawnNodes.append(node);
    }
}

Node* Graph::getNode(int id) {
    return nodes.contains(id) ? nodes[id] : nullptr;
}

void Graph::addEdge(int start, int end, double weight) {
    edges.push_back(Edge(start, end, weight));
}

void Graph::addTrafficLight(int nodeId, TrafficLight* light) {
    trafficLights[nodeId] = light;
}

TrafficLight* Graph::getTrafficLightAtNode(int nodeId) {
    return trafficLights.value(nodeId, nullptr);
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

        for (const Edge& e : edges) {
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
    if (!prev.contains(end) && start != end)
        return path;

    for (int at = end; at != start; at = prev[at]) {
        path.prepend(at);
    }
    path.prepend(start);
    return path;
}
