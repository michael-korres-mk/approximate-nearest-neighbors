//
// Created by mkorres on 10/11/2024.
//

#ifndef GRAPH_H
#define GRAPH_H

#pragma once
#include <vector>

#include "../Edge/Edge.h"
#include "../Vertex/Vertex.h"
#include "../DataSet/DataSet.h"

using namespace std;

template<typename T>
class Graph {
    vector<Vertex<T>> vertices;
    int vertexCount;
    vector<Edge<T>> edges;
    int edgesCount;

public:
    Graph<T>::Graph(DataSet<T> dataSet) ;
    int size();
    void insertVertex(Vertex<T> vertex);
    vector<Vertex<T>> getVertices();
    void removeVertex(Vertex<T> vertex);
    void insertEdge(Vertex<T> src, Vertex<T> dest, float weight);
    void removeEdge(Vertex<T> src, Vertex<T> dest);
    int getWeight(Vertex<T> src, Vertex<T> dest);
    vector<Vertex<T>> getNeighbors(Vertex<T> vertex);
    void search(Vertex<T> vertex);
    ~Graph();
};


#endif //GRAPH_H
