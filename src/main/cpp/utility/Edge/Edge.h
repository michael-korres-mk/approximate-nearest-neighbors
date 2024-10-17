#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>

using namespace std;

class Edge{
    int destination;
    float weight;
public:
    Edge(int destination, float weight);
    int getDestination() const;
    float getWeight() const;
};

#endif // EDGE_HPP
