#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>

using namespace std;

class Edge{
public:
    int destination;
    double weight;
    Edge(int destination, float weight);
};

#endif // EDGE_HPP
