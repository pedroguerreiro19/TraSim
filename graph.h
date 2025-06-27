#ifndef GRAPH_H
#define GRAPH_H

#include "road.h"
#include <QMap>
#include <QVector>
#include <QPointF>

// Forward declaration para evitar dependências circulares
class TrafficLight;

// Tipo de nó no grafo
enum class NodeType {
    Spawn,      // Nó de spawm
    Despawn,    // Nó de despawn
    Ordinary,   // Nó normal
    Light,      // Nó com semáforo
    Yield       // Nó com sinal de cedência
};

// Estrutura que representa um nó do grafo
struct Node {
    int id;
    QPointF position;
    NodeType type;
    RoadType roadtype;

    Node(int id, QPointF pos, NodeType t, RoadType rt)
        : id(id), position(pos), type(t), roadtype (rt) {}
};

// Estrutura que representa uma aresta do grafo
struct Edge {
    int startNode;
    int endNode;
    double weight;

    Edge(int start, int end, double w)
        : startNode(start), endNode(end), weight(w) {}
};

// Classe principal que representa o grafo da simulação
class Graph {
public:
    // Estrutura de dados do grafo
    QMap<int, Node*> nodes;                     // Mapeamento ID
    QVector<Edge> edges;                        // Lista de arestas
    QVector<Node*> spawnNodes;                 // Nós de spawn
    QVector<Node*> despawnNodes;               // Nós de despawn
    QMap<int, TrafficLight*> trafficLights;      // Mapeamento de semáforos

    // Mapeamento de nodes
    QHash<int, Road*> nodeToRoad;
    QVector<Road*> roads;

    // Métodos principais
    void addNode(int id, QPointF pos, NodeType type, RoadType roadtype);
    void addEdge(int start, int end, double weight);
    void addTrafficLight(int nodeId, TrafficLight* light);
    Node* getNode(int id);

    TrafficLight* getTrafficLightAtNode(int nodeId);
    QVector<int> dijkstra(int start, int end);  // Caminho mais curto
};

#endif // GRAPH_H
