#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"
#include "../Utils/Utils.h"


template <typename T>
Graph<T>::Graph(vector<vector<T>> vecs, const int L, const int R,float a):AUTO_INCREMENT(0),L(L),R(R),k(100),a(2){ // TODO: remove k from here
    for(int i = 0; i < vecs.size() ; i++ ) {
        vertexMap.insert({AUTO_INCREMENT,vecs[i]});
        AUTO_INCREMENT++;
    }

    vamana();

}

template <typename T>
void Graph<T>::robustPrune(int p, vector<int>& V,float a,int R){
    return;
}

template <typename T>
void Graph<T>::vamana(){
    initializeRandomEdges();
    int s = medoid();
    auto sigma = Utils<T>::shuffle;

    vector<int> L;
    vector<int> V;
    for(auto x: vertexMap){
        auto vertices = edgesToVertices(g[x.first]);
        pair<vector<int>,vector<int>> gS = greedySearch(s,sigma(vertices),1);
        L = gS.first;
        V = gS.second;

        vector<int> neighbors = getVerticesIds();
        // robustPrune(?,V,a,R);

        for(auto y: edgesToVertices(g[x.first])){
            if(g[y].size() > R){ // TODO: Fix the condition
                // robustPrune(y,g[y],a,R);
            }
            else{
                // g[y.first].push_back(y); // TODO: Fix this
            }
        }
    }

}

template <typename T>
void Graph<T>::initializeRandomEdges(){
    for(auto pair : vertexMap) {
        vector<Edge> neighbors = randomNeighbors(pair.first,k);
        g.insert({pair.first,neighbors});
        if(pair.first > 0 && (pair.first+1) % 1000 == 0) cout<< pair.first + 1 << " nodes' neighbors have been calculated"<< endl;
    }
}

template <typename T>
vector<Edge> Graph<T>::randomNeighbors(int pId,int R) {
    vector<Edge> neighbors;
    vector<int> vertexIds;

    for (const auto& pair : vertexMap) {
        if (pair.first != pId) vertexIds.push_back(pair.first);
    }

    Utils<int>::shuffle(vertexIds);


    vector<int> p = vertexMap[pId];
    for (int i = 0; i < min(R, static_cast<int>(vertexIds.size())); ++i) {
        vector<int> vec = vertexMap[vertexIds[i]];
        neighbors.push_back(Edge(vertexIds[i], euclideanDistance(p,vec)));
    }

    return neighbors;
}


template <typename T>
int Graph<T>::medoid(){
    map<int, float> sumOfDistances;

    for(auto x_i : vertexMap) {
        sumOfDistances[x_i.first] = 0;
        for (auto x_j : vertexMap){
            sumOfDistances[x_i.first] += euclideanDistance(x_i.second, x_j.second);
        }
    }

    auto comparator = [&](const pair<int, float>& a, const pair<int, float>& b) {
        return a.second < b.second; // Sort by distance in ascending order
    };

    vector<pair<int, float>> distanceVec(sumOfDistances.begin(), sumOfDistances.end());

    sort(distanceVec.begin(), distanceVec.end(), comparator);

    return distanceVec.begin()->first;
}

template <typename T>
void Graph<T>::addEdge(int src, int dest,float dist){
    vector<Edge>& srcNeighbors = g[src];
    srcNeighbors.push_back(Edge(dest,dist));
}

template <typename T>
void Graph<T>::addVertex(vector<T> vertex){
    vertexMap.insert({AUTO_INCREMENT,vertex});
    g.insert({AUTO_INCREMENT,vector<Edge>()});
    AUTO_INCREMENT++;
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
    if(v1.size() == 0 || v2.size() == 0) return false;
    const int dim = v1.size();
    for(int i = 0; i < dim; i++){
        if(v1[i] != v2[i]) {
            if(i == dim - 1 || v2[i+1] != v2[i+1]) { // lookahead for equally distant neighbors handling
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
        kNearest.emplace_back(distances[i].first,distances[i].second);
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
int Graph<T>::argmindist(const vector<T>& p, const set<int>& P) {
    float minDist = numeric_limits<float>::max();
    int pStar = -1;
    for(auto p2 : P){
        if(float dist = euclideanDistance(p,vertexMap[p2]); dist < minDist){
            minDist = dist;
            pStar = p2;
        }
    }

    return pStar;
}

template <typename T>
vector<int> Graph<T>::getVerticesIds() {
    vector<int> keys;
    for (const auto& pair : vertexMap) keys.push_back(pair.first);
    return keys;
}

template<typename T>
pair<vector<int>,vector<int>> Graph<T>::greedySearch(int s, const vector<T>& q,int k) {
    vector<Edge> edges{Edge(s,euclideanDistance(q,vertexMap[0]))};
    set<int> L{s};
    set<int> V;
    set<int> diff = setDiff(L, V);

    while(!diff.empty()){
        int pStar = argmindist(q,diff);

        vector<Edge> neighbors = g[pStar];

        for (Edge n : neighbors)
        if(!L.contains(n.getDestination())){
            L.insert(n.getDestination());
            edges.push_back(n);
        }

        V.insert(pStar);

        diff = setDiff(L,V);
    }

    auto comparator = [&](const Edge& a, const Edge& b) {
        return a.getWeight() < b.getWeight(); // Sort by distance in ascending order
    };

    sort(edges.begin(), edges.end(), comparator);

    vector<Edge> kNearest;

    for (int i = 0; i < k && i < edges.size(); i++) {
        kNearest.emplace_back(edges[i]);
    }

    vector<int> visitedVec;
    for(auto v : V) visitedVec.push_back(v);

    return {edgesToVertices(kNearest),visitedVec};
}

template<typename T>
set<int> Graph<T>::setDiff(set<int>& A,set<int>& B){
    set<int> diff;
    for(auto a : A){
        if(find(B.begin(), B.end(), a) == B.end()){
            diff.insert(a);
        }
    }
    return diff;
}

template<typename T>
vector<int> Graph<T>::edgesToVertices(vector<Edge> edges) {
    vector<int> v;
    // Use a lambda to extract destinations and push to v
    transform(edges.begin(), edges.end(), back_inserter(v), [](const Edge& e) {
        return e.getDestination(); // Return the destination to be inserted into v
    });
    return v;
}

template<typename T>
vector<Edge> Graph<T>::getNeighbors(int vertex) {
    return g[vertex];
}


template <typename T>
void Graph<T>::printVector(int id,ostream& out) {
    out << "Vertex[" << id << "] = ";

    for(int j = 0; j < vertexMap[id].size(); j++) {
        out << vertexMap[id][j] << " ";
    }
    out<< endl;
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
    if(!neighbors.empty()) {
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
