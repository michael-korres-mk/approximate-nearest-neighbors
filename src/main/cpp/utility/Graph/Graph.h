#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <map>
#include <vector>
#include "../DataSet/DataSet.h"
#include "../Edge/Edge.h"
#include "../DataSet/DataSet.h"
#include "../SortedContainer/SortedContainer.h"
#include <set>

using namespace std;

template <typename T>
class Graph {
    int AUTO_INCREMENT;             // Χρησιμοποιείται για την ανάθεση μοναδικών ID σε κάθε κορυφή. Ξεκινά από το 0 και αυξάνεται αυτόματα κατά την εισαγωγή κάθε νέας κορυφής.
    map<int, vector<T>> vertexMap;
    map<int,vector<Edge>> g;
    int R;                          // Μέγιστος αριθμός εξερχόμενων ακμών
    int k;                          // Αριθμός γειτόνων που θα βρούμε
    double a;                       // Παράμετρος για το RobustPrune (κατώφλι απόστασης)
public:
    Graph(vector<vector<T>> vecs,int L,int R,double a);
    void vamana();
    void initializeRandomEdges();
    vector<Edge> randomNeighbors(int pId, int R);
    int medoid();
    void addVertex(vector<T> vertex);
    void addEdge(int src, int dest,float dist);
    void removeEdge(int src, int dest);
    vector<Edge> calculateNearestNeighbors(const vector<T>& q,const int& k);
    vector<Edge> robustPrune(int p, const std::vector<int> &V, double a, int R);
    int argminDist(const vector<T>& p, const vector<int>& P);
    int argmindist(const vector<T>& p, const set<int>& P);
    vector<int> getVerticesIds();
    pair<vector<int>,vector<int>> greedySearch(int s, const vector<T>& q,int k, int L);
    set<int> setDiff(SortedContainer& A, set<int>& B);
    vector<int> edgesToVertices(vector<Edge> edges);
    vector<Edge> getNeighbors(int vertex);
    void printVector(int id, ostream& out = cout);
    void printGraph(ostream& out = cout);
    void printVectorNeighbors(vector<Edge>& neighbors, ostream &out = cout);

    // static
    static float euclideanDistance(const vector<T>& v1,const vector<T>& v2);
    static void printVector(pair<int,vector<T>>,ostream& out = cout);
    static bool equals(vector<T> &v1, vector<T> &v2);
};

#include "Graph.cpp"

#endif // GRAPH_HPP
