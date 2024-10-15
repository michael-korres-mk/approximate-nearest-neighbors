//
// Created by mkorres on 10/11/2024.
//

#include "Vertex.h"

template<typename T>
Vertex<T>::Vertex(int &id, const vector<T> &vector):id(id),vec(vector) {}

template<typename T>
int Vertex<T>::getId() const {
    return id;
}

template<typename T>
vector<T> Vertex<T>::getVector() const {
    return vec;
}

template<typename T>
vector<Edge<T>> Vertex<T>::getNeighbors() const {
    return neighbors;
}

// explicit template instantiations
template class Vertex<float>;
template class Vertex<int>;
template class Vertex<char>;
