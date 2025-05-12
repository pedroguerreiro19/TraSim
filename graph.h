#ifndef GRAPH_H
#define GRAPH_H

#include <QMap>
#include <QVector>
#include <QPointF>

class TrafficLight;

enum class NodeType {
    Spawn,
    Despawn,
    Ordinary,
    Light,
    PreLight,
};

struct Node {
    int id;
    QPointF position;
    NodeType type;

    Node(int id, QPointF pos, NodeType t) : id(id), position(pos), type(t) {}
};

struct Edge {
    int startNode;
    int endNode;
    double weight;

    Edge(int start, int end, double w) : startNode(start), endNode(end), weight(w) {}
};


class Graph {
public:
    QMap<int, Node*> nodes;
    QVector<Edge> edges;
    QVector<Node*> spawnNodes;
    QVector<Node*> despawnNodes;
    QMap<int, TrafficLight*> trafficLights;

    void addNode(int id, QPointF pos, NodeType type);
    void addEdge(int start, int end, double weight);
    void addTrafficLight(int nodeId, TrafficLight* light);

    TrafficLight* getTrafficLightAtNode(int nodeId);
    QVector<int> dijkstra(int start, int end);
};

#endif // GRAPH_H
