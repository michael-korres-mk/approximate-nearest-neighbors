//
// Created by mkorres on 10/11/2024.
//

#include "Edge.h"

template<typename T>
Edge<T>::Edge(const Vertex<T> &src, const Vertex<T> &dest, float &distance): src(src),dest(dest),distance(distance) {}

template<typename T>
Vertex<T> Edge<T>::getSrc() const {
    return src;
}

template<typename T>
Vertex<Edge<T>> Edge<T>::getDest() const {
    return dest;
}

template<typename T>
float Edge<T>::getDistance() const {
    return distance;
}

template<typename T>
Edge<T>::~Edge() {
}

// explicit template instantiations
template class Edge<float>;
template class Edge<int>;
template class Edge<char>;