#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"

using namespace std;

template <typename IDType>
class Graph {
private:
    unordered_map<IDType, Node<IDType>> nodes;

public:
    Graph();
    ~Graph();
    Graph(const vector<vector<IDType>>& vec);       // Νέος κατασκευαστής που δέχεται ένα vector από vectors

    bool insertNode(const IDType& id);
    bool insertEdge(const IDType& sourceId, const IDType& destId);
    bool deleteEdge(const IDType& sourceId, const IDType& destId);
    bool printOutgoingTransactions(const IDType& id);
    bool printIncomingTransactions(const IDType& id);
    void printGraph(std::ostream& out);
};

// Συμπεριλαμβάνουμε το αρχείο υλοποίησης
#include "Graph.cpp"

#endif // GRAPH_HPP
