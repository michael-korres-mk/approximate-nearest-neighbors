//
// Created by mkorres on 10/11/2024.
//

#ifndef EDGE_H
#define EDGE_H
#include "../Vertex/Vertex.h"

template <typename T>
class Edge {
    int dest;
    float distance;
public:
    Edge(const Vertex<T>& src,const Vertex<T>& dest,float& distance);
    Vertex<T> getSrc() const ;
    Vertex<Edge<T>> getDest() const ;
    float getDistance() const ;
    ~Edge();
};



#endif //EDGE_H
