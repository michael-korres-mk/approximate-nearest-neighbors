#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <map>
#include <vector>
#include "../DataSet/DataSet.h"
#include "../Edge/Edge.h"
#include "../DataSet/DataSet.h"
#include <set>

# define K 5
using namespace std;

template <typename T>
class Graph {
    map<int, vector<T> > vertexMap;
    map<int,vector<Edge>> g;
    int k;
public:
    Graph(vector<vector<T>> vecs,int k);
    void addVertex(vector<T> vertex);
    void addEdge(int src, int dest,float dist);
    void removeEdge(int src, int dest);
    vector<Edge> calculateNearestNeighbors(const vector<T>& q,const int& k);
    vector<Edge> getNearestNeighbors(const vector<T> &q);
    vector<int> greedySearch(const vector<T>& q);
    vector<int> robustPrune(int p, const std::vector<int>& V, double a, int R);
    set<int> setDiff(set<int>& A, set<int>& B);
    vector<int> edgesToVertices(vector<Edge> edges);
    vector<Edge> getNeighbors(int vertex);
    void printGraph(ostream& out = cout);
    void printVectorNeighbors(vector<Edge>& neighbors, ostream &out = cout);

    // static
    static float euclideanDistance(const vector<T>& v1,const vector<T>& v2);
    static void printVector(pair<int,vector<T>>,ostream& out = cout);
    static bool equals(vector<T> &v1, vector<T> &v2);
};

#include "Graph.cpp"

#endif // GRAPH_HPP
