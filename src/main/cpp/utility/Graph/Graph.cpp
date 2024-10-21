#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"


template <typename T>
Graph<T>::Graph(vector<vector<T>> vecs, const int k):k(k){
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
    srcNeighbors.push_back(Edge(dest,dist));
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
vector<int> Graph<T>::greedySearch(const vector<T>& q) {
    vector<Edge> L{Edge(0,euclideanDistance(q,vertexMap[0]))};
    set<int> setL{0};
    set<int> visited;
    set<int> diff = setDiff(setL, visited);

    while(!diff.empty()){
        int neighbor = -1;
        float minDist = numeric_limits<float>::max();
        for(auto it : diff){
            float dist = euclideanDistance(q,vertexMap[it]);
            if(dist < minDist){
                minDist = dist;
                neighbor = it;
            }
        }

        vector<Edge> neighbors = g[neighbor];

        for (Edge n : neighbors) {

            if(!setL.contains(n.getDestination())){
                setL.insert(n.getDestination());
                L.push_back(n);
            }

        }

        visited.insert(neighbor);

        diff = setDiff(setL,visited);
    }

    auto comparator = [&](const Edge& a, const Edge& b) {
        return a.getWeight() < b.getWeight(); // Sort by distance in ascending order
    };

    sort(L.begin(), L.end(), comparator);

    vector<Edge> kNearest;

    for (int i = 0; i < k && i < L.size(); i++) {
        kNearest.emplace_back(L[i]);
    }

    return edgesToVertices(kNearest);
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

// επιστρέφει μια λίστα που περιέχει τα id των γειτόνων που επιλέχθηκαν μετά τη διαδικασία του pruning
// δέχεται:     1) p -> Το σημείο για το οποίο εκτελείται το pruning (ο κόμβος του γράφου).
//              2) V -> Το σύνολο των υποψήφιων γειτόνων που θα εξεταστούν για το pruning.
//              3) a -> Ένα κατώφλι (threshold) για την απόσταση, με τιμή μεγαλύτερη ή ίση του 1. Χρησιμοποιείται για να κρίνει αν θα αφαιρεθεί κάποιος γείτονας.
//              4) R -> Το μέγιστο όριο γειτόνων που μπορούμε να διατηρήσουμε (degree bound).
template <typename T>
vector<int> Graph<T>::robustPrune(int p, const vector<int>& V, double a, int R) {
    // Αντιγραφή του συνόλου V, αφού θα το τροποποιήσουμε και αφαίρεση του p από το σύνολο των υποψήφιων γειτόνων
    vector<int> candidateNeighbors = V;
    candidateNeighbors.erase(remove(candidateNeighbors.begin(), candidateNeighbors.end(), p), candidateNeighbors.end());

    vector<int> N_out; // Νέοι εξωτερικοί γείτονες

    // Ενώ υπάρχουν υποψήφιοι γείτονες
    while (!candidateNeighbors.empty()) {
        // Βρίσκουμε τον γείτονα που έχει την ελάχιστη απόσταση από το p
        auto minIt = min_element(candidateNeighbors.begin(), candidateNeighbors.end(),
            [&](int pPrime1, int pPrime2) {
                return euclideanDistance(vertexMap[p], vertexMap[pPrime1]) < euclideanDistance(vertexMap[p], vertexMap[pPrime2]);
            });

        int p_star = *minIt; // Ο κοντινότερος γείτονας
        N_out.push_back(p_star); // Προσθήκη του στο σύνολο των νέων γειτόνων
        candidateNeighbors.erase(minIt); // Αφαίρεση του από τους υποψήφιους

        // Αν το πλήθος των νέων γειτόνων φτάσει το όριο R, σταματάμε
        if (N_out.size() == R) {
            break;
        }

        // Κλαδεύουμε τους υπόλοιπους υποψήφιους γείτονες
        for (auto it = candidateNeighbors.begin(); it != candidateNeighbors.end();) {
            if (a * euclideanDistance(vertexMap[p], vertexMap[p_star]) <= euclideanDistance(vertexMap[p], vertexMap[*it])) {
                it = candidateNeighbors.erase(it); // Αφαίρεση των γειτόνων που δεν πληρούν τα κριτήρια
            } else {
                ++it;
            }
        }
    }

    return N_out; // Επιστροφή των επιλεγμένων γειτόνων
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
