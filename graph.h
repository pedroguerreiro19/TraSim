#ifndef GRAPH_H
#define GRAPH_H

#include <QMap>
#include <QVector>
#include <QPointF>

struct Node {
    int id;
    QPointF position;
    Node(int id, QPointF pos) : id(id), position(pos) {}
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

    void addNode(int id, QPointF pos);
    void addEdge(int start, int end, double weight);

    QVector<int> dijkstra(int start, int end);
};

#endif // GRAPH_H
