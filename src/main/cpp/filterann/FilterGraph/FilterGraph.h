//
// Created by mkorres on 11/11/2024.
//

#ifndef FILTERGRAPH_H
#define FILTERGRAPH_H

#include "../DataPoint/DataPoint.h"
#include "../../utility/Utils/Utils.h"
#include "../../utility/Edge/Edge.h"
#include "../../utility/VamanaContainer/VamanaContainer.h"

#include <map>
#include <functional>
#include <set>
#include <chrono>
#include <cmath>

template <typename T>
class FilterGraph{
public:
    int AUTO_INCREMENT;             // Χρησιμοποιείται για την ανάθεση μοναδικών ID σε κάθε κορυφή. Ξεκινά από το 0 και αυξάνεται αυτόματα κατά την εισαγωγή κάθε νέας κορυφής.
    pmr::map<int, DataPoint<T>> vertexMap;
    map<int,vector<Edge>> g;
    int L;
    int R;                          // Μέγιστος αριθμός εξερχόμενων ακμών
    int k;                          // Αριθμός γειτόνων που θα βρούμε
    int d;
    double a;                       // Παράμετρος για το RobustPrune (κατώφλι απόστασης)
    int tau;                        // Παράμετρος για τη δειγματοληψία

    FilterGraph(vector<DataPoint<T>> dataPoints,int L,int R,int k,double a, int tau);
    void initializeRandomEdges();
    vector<Edge> randomNeighbors(int pId, int R);
    void addVertex(DataPoint<T> vertex);
    void addEdge(int src, int dest,float dist);
    void removeEdge(int src, int dest);

    int medoid();
    map<int, int> findMedoid();
    pair<vector<int>,vector<int>> filteredGreedySearch(const vector<int>& S, const vector<T>& q,int k, int L,int Fq);
    void filteredVamana();
    void stitchedVamana();
    vector<Edge> filteredRobustPrune(int p, const vector<int> &V, double a, int R);

    int argminDist(const vector<T>& p, const vector<int>& P);
    int argmindist(const vector<T>& p, const set<int>& P);
    vector<int> getVerticesIds();
    set<int> setDiff(VamanaContainer& A, set<int>& B);
    vector<int> edgesToVertices(vector<Edge> edges);
    vector<Edge> getNeighbors(int vertex);
    void printVector(int id, ostream& out = cout);
    void print(ostream& out = cout);
    void printVectorNeighbors(vector<Edge>& neighbors, ostream &out = cout);
    DataPoint<T> getVertex(int id);

    // static
    static float euclideanDistance(const vector<T>& v1,const vector<T>& v2);
    static void printVector(pair<int,vector<T>>,ostream& out = cout);
    static double equals(const vector<T> &v1, vector<T> &v2);

    // import-export graph
    void importFilterGraph();
    void exportFilterGraph();
};



#endif //FILTERGRAPH_H
