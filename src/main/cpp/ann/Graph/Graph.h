#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <map>
#include <vector>
#include "../../utility/DataSet/DataSet.h"
#include "../../utility/Edge/Edge.h"
#include "../../utility/VamanaContainer/VamanaContainer.h"
#include "../../utility/Utils/Utils.h"

#include <chrono>
#include <set>

using namespace std;

template <typename T>
class Graph {
public:
    int AUTO_INCREMENT;             // Χρησιμοποιείται για την ανάθεση μοναδικών ID σε κάθε κορυφή. Ξεκινά από το 0 και αυξάνεται αυτόματα κατά την εισαγωγή κάθε νέας κορυφής.
    map<int, vector<T>> vertexMap;
    map<int,vector<Edge>> g;
    int L;
    int R;                          // Μέγιστος αριθμός εξερχόμενων ακμών
    int k;                          // Αριθμός γειτόνων που θα βρούμε
    int d;
    double a;                       // Παράμετρος για το RobustPrune (κατώφλι απόστασης)
    Graph(vector<vector<T>> vecs,int L,int R,int k,double a);
    void vamana();
    void initializeRandomEdges();
    vector<Edge> randomNeighbors(int pId, int R);
    int medoid();
    void addVertex(vector<T> vertex);
    void addEdge(int src, int dest,float dist);
    void removeEdge(int src, int dest);
    vector<Edge> calculateNearestNeighbors(const vector<T>& q,const int& k);
    vector<Edge> robustPrune(int p, const std::vector<int> &V, double a, unsigned int R);
    int argminDist(const vector<T>& p, const vector<int>& P);
    int argmindist(const vector<T>& p, const set<int>& P);
    vector<int> getVerticesIds();
    pair<vector<int>,vector<int>> greedySearch(int s, const vector<T>& q,int k, int L);
    set<int> setDiff(VamanaContainer& A, set<int>& B);
    vector<int> edgesToVertices(vector<Edge> edges);
    vector<Edge> getNeighbors(int vertex);
    void printGraph(ostream& out = cout);
    void printVectorNeighbors(vector<Edge>& neighbors, ostream &out = cout);
    vector<T> getVertex(int id);

    // static
    static float euclideanDistance(const vector<T>& v1,const vector<T>& v2);
    static double equals(const vector<T> &v1, vector<T> &v2);

    // For testing
    int getTotalVertices() const;
    const vector<T>& getVertexData(int id) const;
    const vector<Edge>& getEdges(int node) const;
    int getTotalEdges() const;
    const map<int, vector<T>>& getVertexMap() const {
        return vertexMap;
    }

    // import-export graph
    void importGraph();
    void exportGraph();
};


#endif // GRAPH_HPP
