#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"
#include "../Utils/Utils.h"


template <typename T>
Graph<T>::Graph(vector<vector<T>> vecs, const int L,const int R,const int k,double a):AUTO_INCREMENT(0),R(R),k(k),a(a),L(L){
    for(int i = 0; i < vecs.size() ; i++ ) {
        vertexMap.insert({AUTO_INCREMENT,vecs[i]});
        AUTO_INCREMENT++;
    }
}

template <typename T>
void Graph<T>::vamana(){
    cout << "vamana started" << endl;

    initializeRandomEdges();

    int s = medoid();
    auto sigma = Utils<T>::shuffle;

    cout << "medoid calculated" << endl;

    vector<int> V;
    for(const auto& [vertex,neighbors] : vertexMap){
        (vertex % 1000 == 0) && printf("%d\n",vertex);

        const auto& [L,V] = greedySearch(s,neighbors,k,this->L);

        vector<int> neighborIds = getVerticesIds();
        g[vertex] = robustPrune(vertex,V,a,R);

        for(auto y: edgesToVertices(g[vertex])){
            if(g[y].size() + 1 > R){
                vector<int> V = edgesToVertices(g[y]);
                V.push_back(vertex);
                g[y] = robustPrune(y,V,a,R);
            }
            else{
                g[y].push_back(Edge(vertex,euclideanDistance(vertexMap[y],neighbors)));
            }
        }
    }

}

template <typename T>
void Graph<T>::initializeRandomEdges(){
    clock_t start = clock();
    for(const auto& [key, value] : vertexMap) {
        vector<Edge> neighbors = randomNeighbors(key,R);
        g.insert({key,neighbors});
        // if(pair.first > 0 && (pair.first+1) % 1000 == 0) cout<< pair.first + 1 << " nodes' neighbors have been calculated"<< endl;
    }

    clock_t end = clock();
    double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("graph initialization: %f seconds\n", timeSpent);
}

template <typename T>
vector<Edge> Graph<T>::randomNeighbors(int pId,int R) {
    vector<Edge> neighbors;

    vector<T> p = vertexMap[pId];
    int randomId = Utils<int>::random(0,AUTO_INCREMENT - 1);

    set<int> added;

    for (int i = 0; i < R; ++i) {
        while (randomId == pId || added.find(randomId) != added.end()) {
            randomId = Utils<int>::random(0,AUTO_INCREMENT - 1);
        }

        added.insert(randomId);
        vector<T> vec = vertexMap[randomId];
        neighbors.push_back(Edge(randomId, euclideanDistance(p,vec)));
    }

    return neighbors;
}


// Μειώνεται η πολυπλοκότητα από O(n^2) σε O(m^2), όπου m είναι το μέγεθος του δείγματος (m << n).
// Το δείγμα θα πρέπει να είναι περίπου το 10% του συνολικού αριθμού των κόμβων
template <typename T>
int Graph<T>::medoid() {
    int n = vertexMap.size();
    int sample_size = n/10;
    sample_size = min(sample_size, n);

    // Επιλέγουμε τυχαία ένα δείγμα από τα IDs των σημείων
    vector<int> sample_ids;
    for (const auto& [id, vec] : vertexMap) {
        sample_ids.push_back(id);
    }
    random_shuffle(sample_ids.begin(), sample_ids.end());
    sample_ids.resize(sample_size);

    int medoid_id = -1;
    double min_total_distance = numeric_limits<double>::infinity();

    // Υπολογίζουμε το medoid στο δείγμα
    for (int id_i : sample_ids) {
        double total_distance = 0.0;
        for (int id_j : sample_ids) {
            if (id_i != id_j) {
                total_distance += euclideanDistance(vertexMap[id_i], vertexMap[id_j]);
            }
        }
        if (total_distance < min_total_distance) {
            min_total_distance = total_distance;
            medoid_id = id_i;
        }
    }
    return medoid_id;
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
double Graph<T>::equals(const vector<T>& v1, vector<T>& v2) {
    if(v1.size() == 0 || v2.size() == 0) return false;
    const int dim = v1.size();

    int misses = 0;

    for(int i = 0; i < dim; i++){
        if(v1[i] != v2[i]) {
            if(i == dim - 1 || v2[i+1] != v2[i+1]) { // lookahead for equally distant neighbors handling
                misses++;
            }
            else {
                i++; // do not check next
            }
        }

    }

    cout << "misses: " << misses << endl;
    cout << "dim: " << dim << endl;

    return (static_cast<double>(dim) - misses) / dim;
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
pair<vector<int>,vector<int>> Graph<T>::greedySearch(int s, const vector<T>& q, const int k, int L) {

    // clock_t start = clock();

    VamanaContainer l(L); l.insert({s,euclideanDistance(q,vertexMap[s])});      // Σύνολο αναζήτησης
    set<int> V;         // Σύνολο επισκεφθέντων κόμβων
    set<int> diff = setDiff(l, V);

    while(!diff.empty()){
        int pStar = argmindist(q,diff);   // Βρίσκουμε το p* (τον πιο κοντινό κόμβο που δεν έχει επισκεφτεί ακόμα)

        vector<Edge> neighbors = g[pStar];  // Βρίσκουμε τους γείτονες του p*

        // Ενημερώνουμε το L με τους νέους γείτονες
        for (Edge n : neighbors) {
            l.insert({n.getDestination(),n.getWeight()});
        }

        V.insert(pStar);    // Προσθέτουμε το p* στο σύνολο επισκέψεων
        diff = setDiff(l,V);

    }

    // Μετατροπή των αποτελεσμάτων για επιστροφή
    vector<int> visitedVec(V.begin(), V.end());

    // clock_t end = clock();
    // double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
    // printf("greedy search: %f seconds\n", timeSpent);

    return {l.subset(k), visitedVec};
}

template<typename T>
set<int> Graph<T>::setDiff(VamanaContainer& A,set<int>& B){
    set<int> diff;
    vector<int> itemsOfA = A.subset(-1);
    for(auto a : itemsOfA){
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

template<typename T>
int Graph<T>::argminDist(const vector<T>& p, const vector<int>& P) {
    double minDist = std::numeric_limits<double>::max();
    int pStar = -1;
    for (int p2 : P) {
        double dist = euclideanDistance(p, vertexMap[p2]);
        if (dist < minDist) {
            minDist = dist;
            pStar = p2;
        }
    }
    return pStar;
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
        int p_star = argminDist(vertexMap[p], candidateNeighbors); // Χρήση της συνάρτησής σου

        // Προσθήκη του p_star στους νέους γείτονες
        N_out.push_back(Edge(p_star, euclideanDistance(vertexMap[p], vertexMap[p_star])));

        // Αφαίρεση του p_star από τους υποψήφιους
        candidateNeighbors.erase(std::remove(candidateNeighbors.begin(), candidateNeighbors.end(), p_star), candidateNeighbors.end());


        // Αν το πλήθος των νέων γειτόνων φτάσει το όριο R, σταματάμε
        if (N_out.size() == R) {
            break;
        }

        // Κλαδεύουμε τους υπόλοιπους υποψήφιους γείτονες
        for (auto it = candidateNeighbors.begin(); it != candidateNeighbors.end();) {
            if (a * euclideanDistance(vertexMap[p_star], vertexMap[*it]) <= euclideanDistance(vertexMap[p], vertexMap[*it])) {
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

            out << "[" << neighbor.getDestination() << "] = ";
            for(int j = 0; j < vertexMap[neighbor.getDestination()].size(); j++) {
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
