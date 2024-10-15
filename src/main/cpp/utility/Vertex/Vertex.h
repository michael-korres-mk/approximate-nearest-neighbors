//
// Created by mkorres on 10/11/2024.
//

#ifndef VERTEX_H
#define VERTEX_H

# include <iomanip>
# include <valarray>
# include <vector>
# include <cassert>
# include <fstream>
# include "../Edge/Edge.h"

using namespace std;

template <typename T>
class Vertex {
    int id;
    vector<T> vec;
    vector<Edge<int>> neighbors;
    // + outneighbors
public:
    Vertex(int& id, const vector<T>& vector);
    int getId() const ;
    vector<T> getVector() const ;
    vector<Edge<T>> getNeighbors() const ;
};



#endif //VERTEX_H
