//
// Created by mkorres on 10/11/2024.
//

#include "Graph.h"

template<typename T>
Graph<T>::Graph(DataSet<T> dataSet) {

    vector<vector<T>> vecs = dataSet.getVectors();

    for (auto vec: vecs) {

        Vertex<T> vertex(vec);

        vector<int> neighbors = dataSet.getNearestNeighbors(vec);

        vertex.insertNeighbors(neighbors);

        insertVertex(vertex);
    }

}

template<typename T>
int Graph<T>::size() {
    return vertices.size();
}

template<typename T>
void Graph<T>::insertVertex(Vertex<T> vertex) {
    vertices.push_back(vertex);
}

template<typename T>
vector<Vertex<T>> Graph<T>::getVertices() {
    return vertices;
}

template<typename T>
void Graph<T>::removeVertex(Vertex<T> vertex) {
    vertices.erase(find(vertices.begin(), vertices.end(), vertex));
}

template<typename T>
void Graph<T>::insertEdge(Vertex<T> src, Vertex<T> dest, float weight) {
    vertices[src].insertEdge(dest, weight);
}

template<typename T>
void Graph<T>::removeEdge(Vertex<T> src, Vertex<T> dest) {
    vertices[src].removeEdge(dest);
}

template<typename T>
int Graph<T>::getWeight(Vertex<T> src, Vertex<T> dest) {
    return vertices[src].getWeight(dest);
}

template<typename T>
vector<Vertex<T>> Graph<T>::getNeighbors(Vertex<T> vertex) {
    return vertices[vertex].getNeighbors();
}

template<typename T>
void Graph<T>::search(Vertex<T> vertex) {
}

template<typename T>
Graph<T>::~Graph() {
}

// explicit template instantiations
template class Graph<float>;
template class Graph<int>;
template class Graph<char>;