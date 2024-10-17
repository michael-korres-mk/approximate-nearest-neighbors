#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"


template <typename T>
Graph<T>::Graph(vector<vector<T>> vecs,int k){
    for(int i = 0; i < vecs.size() ; i++ ) {
        vertexMap.insert({i,vecs[i]});
    }

    for(int i = 0; i < vecs.size() ; i++ ) {
        vector<Edge> neighbors = calculateNearestNeighbors(vertexMap[i],k);
        g.insert({i,neighbors});
        if(i > 0 && (i+1) % 1000 == 0) cout<< i + 1 << " nodes' neighbors have been calculated"<< endl;
    }
}

template <typename T>
void Graph<T>::addEdge(int src, int dest,float dist){
    vector<Edge>& srcNeighbors = g[src];
    srcNeighbors.emplace_back(dest,dist);
}

template <typename T>
void Graph<T>::addVertex(vector<T> vertex){
    int id = vertexMap.size();
    vertexMap.insert({id,vertex});
    g.insert({id,vector<Edge>()});
}

template <typename T>
void Graph<T>::removeEdge(const int src, const int dest){
    vector<Edge>& srcNeighbors = g[src];
    srcNeighbors.erase(
        remove_if(srcNeighbors.begin(), srcNeighbors.end(),[&](const Edge& e) { return e.getDestination() == dest; }),
        srcNeighbors.end()
    );
}

template<typename T>
float Graph<T>::euclideanDistance(const vector<T>& v1,const vector<T>& v2) {
    int d = v1.size();
    float dist = 0;
    for(int j = 0; j < d; j++) {
        dist += (v2[j] - v1[j]) * (v2[j] - v1[j]);
    }

    return sqrt(dist);
}

template<typename T>
bool Graph<T>::equals(vector<T>& v1, vector<T>& v2) {
    const int dim = v1.size();
    for(int i = 0; i < dim; i++){
        if(v1[i] != v2[i]) {
            if(( i == dim -1) || (v2[i+1] != v2[i+1])) { // lookahead for equally distant neighbors handling
                return false;
            }
            else {
                i++; // do not check next
            }
        }

    }

    return true;
}

template<typename T>
vector<Edge> Graph<T>::calculateNearestNeighbors(const vector<T>& q,const int& k) {
    vector<pair<int, float>> distances;

    for(int i = 0; i < vertexMap.size(); i++) {
        if(float dist = euclideanDistance(q,vertexMap[i]); dist != 0) distances.emplace_back( i ,dist);
    }

    auto comparator = [&](const pair<int, float>& a, const pair<int, float>& b) {
        return a.second < b.second; // Sort by distance in ascending order
    };

    sort(distances.begin(), distances.end(), comparator);

    vector<Edge> kNearest;

    for (int i = 0; i < k && i < distances.size(); i++) {
        kNearest.push_back(Edge(distances[i].first,distances[i].second));
    }

    return kNearest;
}

template<typename T>
vector<Edge> Graph<T>::getNearestNeighbors(const vector<T>& q) {
    pair<int, float> minDistance = {-1,numeric_limits<float>::max()};

    for(int i = 0; i < vertexMap.size(); i++) {
        float dist = euclideanDistance(q,vertexMap[i]);
        if(dist != 0) {
           if(dist < minDistance.second) {
               minDistance = {i, dist};
           }
        }
        else {
            return g[i];
        }
    }



    vector<Edge> kNearest{Edge(minDistance.first,minDistance.second)};
    for(int i = 0; i < g[minDistance.first].size() - 1; i++) kNearest.push_back(g[minDistance.first][i]);
    return kNearest;

}


template<typename T>
vector<Edge> Graph<T>::getNeighbors(int vertex) {
    return g[vertex];
}


template <typename T>
void Graph<T>::printVector(pair<int,vector<T>> pair,ostream& out) {
    const int id = pair.first;
    out << "Vertex[" << id << "] = ";
    for(int j = 0; j < pair.second.size(); j++) {
        out << pair.second[j] << " ";
    }
    out<< endl;
}

template <typename T>
void Graph<T>::printVectorNeighbors(vector<Edge>& neighbors,ostream& out) {

    out << "Neighbors:" << endl;
    if(neighbors.size() > 0) {
        for(Edge neighbor : neighbors){

            for(int j = 0; j < vertexMap[neighbor.getDestination()].size(); j++) {
                out << "[" << neighbor.getDestination() << "] = ";
                out << vertexMap[neighbor.getDestination()][j] << " ";
            }
            out << "(" << neighbor.getWeight() << ")" << " ";
            out << endl;
        }
    }
    else {
        out << "No neighbors" << endl;
    }
}


template <typename T>
void Graph<T>::printGraph(ostream& out){
    out << "Graph with " << vertexMap.size() << " vertices:" << endl;
    for(const auto& pair : vertexMap){
        printVector(pair);
        const int id = pair.first;
        printVectorNeighbors(g[id]);
    }
}

#endif // GRAPH_CPP
