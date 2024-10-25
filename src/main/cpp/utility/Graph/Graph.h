#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <bits/stdc++.h>
#include "../Edge/Edge.h"

using namespace std;

template <typename T>
class Graph {
    map<int, vector<T> > vertexMap;
    map<int,vector<Edge>> g;
    int AUTO_INCREMENT;     // Χρησιμοποιείται για την ανάθεση μοναδικών ID σε κάθε κορυφή. Ξεκινά από το 0 και αυξάνεται αυτόματα κατά την εισαγωγή κάθε νέας κορυφής.
    int R;                          // Μέγιστος αριθμός εξερχόμενων ακμών
    int k;                          // Αριθμός γειτόνων που θα βρούμε
    double a;                       // Παράμετρος για το RobustPrune (κατώφλι απόστασης)
    int L;                         // Παράμετρος για το GreedySearch που καθορίζει το μέγεθος της λίστας αναζήτησης.

public:
    Graph(const vector<vector<T>>& vecs, int R, int k, double a, int L);

    void addVertex(const vector<T>& vertex);
    void addEdge(int src, int dest, float dist);
    vector<int> getOutNeighbors(int vertex);

    void initializeRandomGraph(int R);
    int findMedoid();
    int findMedoidSample(int sample_size);
    vector<int> generateRandomPermutation(int n);

    void vamanaIndexing(const vector<vector<T>>& P, int L, int R, double a);
    vector<int> robustPrune(int p, const vector<int>& V, double a, int R);
    pair<vector<int>, unordered_set<int>> greedySearch(int s, const vector<T>& x_q, int k, int L);

    void setOutNeighbors(int node, const std::vector<int>& neighbors);
    void addOutNeighbor(int node, int neighbor);
    static double euclideanDistance(const vector<T>& v1, const vector<T>& v2);  // Συνάρτηση για υπολογισμό Ευκλείδειας απόστασης

    void printGraph(ostream& out);
    void generateDotFile(const string& filename);
};

#include "Graph.cpp"

#endif // GRAPH_HPP
