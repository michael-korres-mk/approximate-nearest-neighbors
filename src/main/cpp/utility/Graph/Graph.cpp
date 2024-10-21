#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"
#include "../Utils/Utils.h"


template <typename T>
Graph<T>::Graph(vector<vector<T>> vecs, const int R,const int k,double a):AUTO_INCREMENT(0),R(R),k(k),a(a){
    for(int i = 0; i < vecs.size() ; i++ ) {
        vertexMap.insert({AUTO_INCREMENT,vecs[i]});
        AUTO_INCREMENT++;
    }

    vamana();

}

template <typename T>
void Graph<T>::vamana(){
    cout << "vamana started" << endl;

    initializeRandomEdges();
    cout << "random edges initialized" << endl;
    int s = medoid();
    auto sigma = Utils<T>::shuffle;

    cout << "medoid calculated" << endl;

    vector<int> L;
    vector<int> V;
    int loop = 1;
    for(auto x: vertexMap){
        cout << "loop: " << loop << "| point.id = " << x.first << endl;

        auto vertices = edgesToVertices(g[x.first]);
        pair<vector<int>,vector<int>> gS = greedySearch(s,x.second,1);
        cout << "greedy search completed" << endl;
        L = gS.first;
        V = gS.second;

        vector<int> neighbors = getVerticesIds();
        g[x.first] = robustPrune(x.first,V,a,R);
        cout << "robust prune completed" << endl;

        for(auto y: edgesToVertices(g[x.first])){
            if((g[y].size() + 1) > R){
                vector<int> V = edgesToVertices(g[y]);
                V.push_back(x.first);
                g[y] = robustPrune(y,V,a,R);
            }
            else{
                g[y].push_back(Edge(x.first,euclideanDistance(vertexMap[y],x.second)));
            }
        }
        cout << "reverse edge addition completed" << endl;

        loop++;
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


    vector<T> p = vertexMap[pId];
    for (int i = 0; i < min(R, static_cast<int>(vertexIds.size())); ++i) {
        vector<T> vec = vertexMap[vertexIds[i]];
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

// επιστρέφει μια λίστα που περιέχει τα id των γειτόνων που επιλέχθηκαν μετά τη διαδικασία του pruning
// δέχεται:     1) p -> Το σημείο για το οποίο εκτελείται το pruning (ο κόμβος του γράφου).
//              2) V -> Το σύνολο των υποψήφιων γειτόνων που θα εξεταστούν για το pruning.
//              3) a -> Ένα κατώφλι (threshold) για την απόσταση, με τιμή μεγαλύτερη ή ίση του 1. Χρησιμοποιείται για να κρίνει αν θα αφαιρεθεί κάποιος γείτονας.
//              4) R -> Το μέγιστο όριο γειτόνων που μπορούμε να διατηρήσουμε (degree bound).
template <typename T>
vector<Edge> Graph<T>::robustPrune(int p, const vector<int> &V, double a, int R) {
    // Αντιγραφή του συνόλου V, αφού θα το τροποποιήσουμε και αφαίρεση του p από το σύνολο των υποψήφιων γειτόνων
    vector<int> candidateNeighbors = V;
    candidateNeighbors.erase(remove(candidateNeighbors.begin(), candidateNeighbors.end(), p), candidateNeighbors.end());

    vector<Edge> N_out; // Νέοι εξωτερικοί γείτονες

    // Ενώ υπάρχουν υποψήφιοι γείτονες
    while (!candidateNeighbors.empty()) {
        // Βρίσκουμε τον γείτονα που έχει την ελάχιστη απόσταση από το p
        auto minIt = min_element(candidateNeighbors.begin(), candidateNeighbors.end(),
            [&](int pPrime1, int pPrime2) {
                return euclideanDistance(vertexMap[p], vertexMap[pPrime1]) < euclideanDistance(vertexMap[p], vertexMap[pPrime2]);
            });


        int p_star = *minIt; // Ο κοντινότερος γείτονας
        N_out.push_back(Edge(p,euclideanDistance(vertexMap[p],vertexMap[candidateNeighbors[*minIt]]))); // Προσθήκη του στο σύνολο των νέων γειτόνων
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
