//
// Created by mkorres on 11/11/2024.
//

#include "../FilterGraph/FilterGraph.h"
#include "../Utils/Utils.h"
#include <functional>



template <typename T>
FilterGraph<T>::FilterGraph(vector<DataPoint<T>> dataPoints, const int L,const int R,const int k,double a, int tau):AUTO_INCREMENT(0),R(R),k(k),a(a),L(L),tau(tau),d((dataPoints.size() > 0)?static_cast<int>(dataPoints[0].vec.size()):0){
    // TODO: Revisit d initialization
    for(int i = 0; i < dataPoints.size() ; i++ ) {
        vertexMap.insert({AUTO_INCREMENT,dataPoints[i]});
        AUTO_INCREMENT++;
    }
}

template <typename T>
void FilterGraph<T>::initializeRandomEdges(){
	auto start = chrono::high_resolution_clock::now();
    for(const auto& [key, value] : vertexMap) {
        vector<Edge> neighbors = randomNeighbors(key,R);
        g.insert({key,neighbors});
        // if(pair.first > 0 && (pair.first+1) % 1000 == 0) cout<< pair.first + 1 << " nodes' neighbors have been calculated"<< endl;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "graph initialization: " << duration << " ms" << endl;
}

template <typename T>
vector<Edge> FilterGraph<T>::randomNeighbors(int pId,int R) {
    vector<Edge> neighbors;

    vector<T> p = vertexMap[pId].vec;
    int randomId = Utils<int>::random(0,AUTO_INCREMENT - 1);

    set<int> added;

    for (int i = 0; i < R; ++i) {
        while (randomId == pId || added.find(randomId) != added.end()) {
            randomId = Utils<int>::random(0,AUTO_INCREMENT - 1);
        }

        added.insert(randomId);
        vector<T> vec = vertexMap[randomId].vec;
        neighbors.push_back(Edge(randomId, euclideanDistance(p,vec)));
    }

    return neighbors;
}

////////

template <typename T>
map<int, int> FilterGraph<T>::findMedoid() {
    // Δημιουργία των χάρτων M και medoidCount
    map<int, int> M;             // Χάρτης που αντιστοιχίζει κάθε φίλτρο στο medoid του
    map<int, int> medoidCount;   // Χάρτης που μετράει πόσες φορές κάθε σημείο είναι medoid
    // Αρχικοποίηση όλων των σημείων στο medoidCount με μηδενικές τιμές
    for (const auto& [id, dataPoint] : vertexMap) {
        medoidCount[id] = 0; // Όλα τα IDs ξεκινούν με μετρητή 0
    }

    // Συλλογή όλων των μοναδικών φίλτρων F
    set<int> F;
    for (const auto& [id, dataPoint] : vertexMap) {
        F.insert(dataPoint.C);  // Προσθήκη των φίλτρων C στο σύνολο F
    }

    // Επεξεργασία κάθε φίλτρου f στο σύνολο F
    for (int f : F) {
        // P_f: Συλλογή όλων των σημείων δεδομένων που σχετίζονται με το φίλτρο f
        vector<int> P_f;
        for (const auto& [id, dataPoint] : vertexMap) {
            if (dataPoint.C == f) {
                P_f.push_back(id);
            }
        }

        // R_f: Τυχαία δειγματοληψία tau σημείων από το P_f
        vector<int> R_f;
        if (P_f.size() <= tau) {
            // Αν τα σημεία είναι λιγότερα ή ίσα με tau, παίρνουμε όλα τα σημεία
            R_f = P_f;
        } else {
            // Τυχαία αναδιάταξη του P_f χρησιμοποιώντας τη συνάρτηση shuffle της Utils
            Utils<int>::shuffle(P_f);
            // Επιλογή των πρώτων tau στοιχείων μετά την αναδιάταξη
            R_f.assign(P_f.begin(), P_f.begin() + tau);
        }

        // Εύρεση του medoid p* που έχει τον ελάχιστο medoidCount[p]
        int p_star = -1;
        int min_medoid_count = numeric_limits<int>::max();

        for (int p_id : R_f) {
            int current_count = medoidCount[p_id];
            if (current_count < min_medoid_count) {
                min_medoid_count = current_count;
                p_star = p_id;
            }
        }

        // Ενημέρωση των χαρτών M και medoidCount
        M[f] = p_star;
        medoidCount[p_star]++;
    }

    // Επιστροφή των χάρτων M και medoidCount
    return M;
}

template<typename T>
pair<vector<int>,vector<int>> FilterGraph<T>::filteredGreedySearch(const vector<int>& S, const vector<T>& q, const int k, const int L,int Fq) {

    VamanaContainer l(L);
    set<int> V;         // Σύνολο επισκεφθέντων κόμβων

    for (auto s : S) {
        if(vertexMap[s].C == Fq)
        l.insert({s,euclideanDistance(q,vertexMap[s].vec)});      // Σύνολο αναζήτησης
    }
    set<int> diff = setDiff(l, V);

    while(!diff.empty()){
        int pStar = argmindist(q,diff);   // Βρίσκουμε το p* (τον πιο κοντινό κόμβο που δεν έχει επισκεφτεί ακόμα)

        vector<Edge> neighbors;  // Βρίσκουμε τους γείτονες του p*

        for(auto& e : g[pStar]) {
            if(vertexMap[e.destination].C == Fq) {
                neighbors.push_back(e);
            }
        }

        // Ενημερώνουμε το L με τους νέους γείτονες
        for (Edge n : neighbors) {
            l.insert({n.destination,n.weight});
        }

        V.insert(pStar);    // Προσθέτουμε το p* στο σύνολο επισκέψεων
        diff = setDiff(l,V);

    }

    // Μετατροπή των αποτελεσμάτων για επιστροφή
    vector<int> visitedVec(V.begin(), V.end());

    return {l.subset(k), visitedVec};
    return {};
}

template <typename T>
void FilterGraph<T>::filteredVamana() {}

template <typename T>
void FilterGraph<T>::stitchedVamana(){}

template <typename T>
vector<Edge> FilterGraph<T>::filteredRobustPrune(int p, const vector<int> &V, double a, int R) {
    // // Αντιγραφή του συνόλου V, αφού θα το τροποποιήσουμε και αφαίρεση του p από το σύνολο των υποψήφιων γειτόνων
    // vector<int> candidateNeighbors = V;
    // vector<Edge> pOut = getNeighbors(p);
    // for (Edge e : pOut) {
    //     if (find(candidateNeighbors.begin(), candidateNeighbors.end(), e.destination) == candidateNeighbors.end()) {
    //         candidateNeighbors.push_back(e.destination);
    //     }
    // }
    // candidateNeighbors.erase(remove(candidateNeighbors.begin(), candidateNeighbors.end(), p), candidateNeighbors.end());
    //
    // vector<Edge> N_out; // Νέοι εξωτερικοί γείτονες
    //
    // // Ενώ υπάρχουν υποψήφιοι γείτονες
    // while (!candidateNeighbors.empty()) {
    //     // Βρίσκουμε τον γείτονα που έχει την ελάχιστη απόσταση από το p
    //     int p_star = argminDist(vertexMap[p], candidateNeighbors); // Χρήση της συνάρτησής σου
    //
    //     // Προσθήκη του p_star στους νέους γείτονες
    //     N_out.push_back(Edge(p_star, euclideanDistance(vertexMap[p], vertexMap[p_star])));
    //
    //     // Αφαίρεση του p_star από τους υποψήφιους
    //     candidateNeighbors.erase(std::remove(candidateNeighbors.begin(), candidateNeighbors.end(), p_star), candidateNeighbors.end());
    //
    //
    //     // Αν το πλήθος των νέων γειτόνων φτάσει το όριο R, σταματάμε
    //     if (N_out.size() == R) break;
    //
    //
    //     // Κλαδεύουμε τους υπόλοιπους υποψήφιους γείτονες
    //     for (auto it = candidateNeighbors.begin(); it != candidateNeighbors.end();) {
    //         if (a * euclideanDistance(vertexMap[p_star], vertexMap[*it]) <= euclideanDistance(vertexMap[p], vertexMap[*it])) {
    //             it = candidateNeighbors.erase(it); // Αφαίρεση των γειτόνων που δεν πληρούν τα κριτήρια
    //         } else {
    //             ++it;
    //         }
    //     }
    // }
    //
    // return N_out; // Επιστροφή των επιλεγμένων γειτόνων
    return {};
}



////////


template <typename T>
void FilterGraph<T>::addEdge(int src, int dest,float dist){
    vector<Edge>& srcNeighbors = g[src];
    srcNeighbors.push_back(Edge(dest,dist));
}

template <typename T>
void FilterGraph<T>::addVertex(DataPoint<T> vertex){
    vertexMap.insert({AUTO_INCREMENT,vertex});
    g.insert({AUTO_INCREMENT,vector<Edge>()});
    AUTO_INCREMENT++;
}

template <typename T>
void FilterGraph<T>::removeEdge(const int src, const int dest){
    vector<Edge>& srcNeighbors = g[src];
    srcNeighbors.erase(
        remove_if(srcNeighbors.begin(), srcNeighbors.end(),[&](const Edge& e) { return e.destination == dest; }),
        srcNeighbors.end()
    );
}

template<typename T>
float FilterGraph<T>::euclideanDistance(const vector<T>& v1,const vector<T>& v2) {
    int d = v1.size();
    float dist = 0;
    for(int j = 0; j < d; j++) {
        dist += (v2[j] - v1[j]) * (v2[j] - v1[j]);
    }

    return sqrt(dist);
}

template<typename T>
double FilterGraph<T>::equals(const vector<T>& v1, vector<T>& v2) {
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

    return (static_cast<double>(dim) - misses) / dim;
}

template <typename T>
vector<int> FilterGraph<T>::getVerticesIds() {
    vector<int> keys;
    for (const auto& pair : vertexMap) keys.push_back(pair.first);
    return keys;
}


template<typename T>
set<int> FilterGraph<T>::setDiff(VamanaContainer& A,set<int>& B){
    set<int> diff;
    vector<int> itemsOfA = A.subset(-1);
    for(auto a : itemsOfA){
        if(B.find(a) == B.end()){
            diff.insert(a);
        }
    }
    return diff;
}

template<typename T>
vector<int> FilterGraph<T>::edgesToVertices(vector<Edge> edges) {
    vector<int> v;
    // Use a lambda to extract destinations and push to v
    transform(edges.begin(), edges.end(), back_inserter(v), [](const Edge& e) {
        return e.destination; // Return the destination to be inserted into v
    });
    return v;
}

template<typename T>
vector<Edge> FilterGraph<T>::getNeighbors(int vertex) {
    return g[vertex];
}

template<typename T>
int FilterGraph<T>::argmindist(const vector<T>& p, const set<int>& P) {
    float minDist = numeric_limits<float>::max();
    int pStar = -1;
    for(auto p2 : P){
        if(float dist = euclideanDistance(p,vertexMap[p2].vec); dist < minDist){
            minDist = dist;
            pStar = p2;
        }
    }

    return pStar;
}

template<typename T>
int FilterGraph<T>::argminDist(const vector<T>& p, const vector<int>& P) {
    double minDist = std::numeric_limits<double>::max();
    int pStar = -1;
    for (int p2 : P) {
        double dist = euclideanDistance(p, vertexMap[p2].vec);
        if (dist < minDist) {
            minDist = dist;
            pStar = p2;
        }
    }
    return pStar;
}

template <typename T>
void FilterGraph<T>::printVector(int id,ostream& out) {
    out << "Vertex[" << id << "] = ";

    for(int j = 0; j < vertexMap[id].vec.size(); j++) {
        out << vertexMap[id].vec[j] << " ";
    }
    out<< endl;
}

template <typename T>
void FilterGraph<T>::printVector(pair<int,vector<T>> pair,ostream& out) {
    const int id = pair.first;
    out << "Vertex[" << id << "] = ";
    for(int j = 0; j < pair.second.size(); j++) {
        out << pair.second[j] << " ";
    }
    out<< endl;
}

template <typename T>
void FilterGraph<T>::printVectorNeighbors(vector<Edge>& neighbors,ostream& out) {

    out << "Neighbors:" << endl;
    if(!neighbors.empty()) {
        for(Edge neighbor : neighbors){

            out << "[" << neighbor.destination << "] = ";
            for(int j = 0; j < vertexMap[neighbor.destination].vec.size(); j++) {
                out << vertexMap[neighbor.destination].vec[j] << " ";
            }
            out << "(" << neighbor.weight << ")" << " ";
            out << endl;
        }
    }
    else {
        out << "No neighbors" << endl;
    }
}

template<typename T>
DataPoint<T> FilterGraph<T>::getVertex(int id) {
    if(id >= vertexMap.size()) return DataPoint<T>();
    return vertexMap[id];
}


// import-export graph
template <typename T>
void FilterGraph<T>::exportFilterGraph() {
    //
    // const string filename = "graph.bin";
    // const string graphFilePath(RESOURCES_P + filename);
    //
    // ofstream file(graphFilePath, ios::binary);
    // if (!file.is_open()) {
    //     throw runtime_error("I/O error: Unable to open the file " + filename);
    // }
    //
    // // persist graph metadata
    // file.write(reinterpret_cast<const char*>(&AUTO_INCREMENT), sizeof(int));
    // file.write(reinterpret_cast<const char*>(&L), sizeof(int));
    // file.write(reinterpret_cast<const char*>(&R), sizeof(int));
    // file.write(reinterpret_cast<const char*>(&k), sizeof(int));
    // file.write(reinterpret_cast<const char*>(&d), sizeof(int));
    // file.write(reinterpret_cast<const char*>(&a), sizeof(double));
    //
    // // persist vector type size
    // size_t typeSize = sizeof(T);
    // file.write(reinterpret_cast<const char*>(&typeSize), sizeof(size_t));
    //
    // size_t numOfNeighbors;
    //
    // // persist vectors
    // for(const auto& [id, vector] : vertexMap) {
    //     file.write(reinterpret_cast<const char*>(&id), sizeof(int));
    //     for(auto& xi : vector) {
    //         file.write(reinterpret_cast<const char*>(&xi), sizeof(typeSize));
    //     }
    //
    //     // persist num of neighbors
    //     numOfNeighbors = g[id].size();
    //     file.write(reinterpret_cast<const char*>(&numOfNeighbors), sizeof(size_t));
    //
    //     // persist neighbors
    //     auto& neighbors = g[id];
    //     for(auto& n : neighbors) {
    //         file.write(reinterpret_cast<const char*>(&n.destination), sizeof(int));
    //         file.write(reinterpret_cast<const char*>(&n.weight), sizeof(int));
    //     }
    // }
    //
    // file.close();

}

template <typename T>
void FilterGraph<T>::importFilterGraph() {

    // const string filename = "graph.bin";
    // const string graphFilePath(RESOURCES_P + filename);
    //
    // ifstream file(graphFilePath, ios::binary);
    // if (!file.is_open()) {
    //     throw runtime_error("I/O error: Unable to open the file " + filename);
    // }
    //
    // // fetch graph metadata
    // file.read(reinterpret_cast<char*>(&AUTO_INCREMENT), sizeof(int));
    // file.read(reinterpret_cast<char*>(&L), sizeof(int));
    // file.read(reinterpret_cast<char*>(&R), sizeof(int));
    // file.read(reinterpret_cast<char*>(&k), sizeof(int));
    // file.read(reinterpret_cast<char*>(&d), sizeof(int));
    // file.read(reinterpret_cast<char*>(&a), sizeof(double));
    //
    // // fetch vector type size
    // size_t typeSize;;
    // file.read(reinterpret_cast<char*>(&typeSize), sizeof(size_t));
    //
    // size_t numOfNeighbors;
    //
    // // fetch vectors
    // for(int i = 0; i < AUTO_INCREMENT; i++) {
    //     int id;
    //     vector<T> vec;
    //
    //     file.read(reinterpret_cast<char*>(&id), sizeof(int));
    //     T xi;
    //     for(int j = 0; j < d; j++) {
    //         file.read(reinterpret_cast<char*>(&xi), sizeof(typeSize));
    //         vec.push_back(xi);
    //     }
    //
    //     vertexMap[id] = vec;
    //
    //     // fetch num of neighbors
    //     file.read(reinterpret_cast<char*>(&numOfNeighbors), sizeof(size_t));
    //
    //     // fetch neighbors
    //     vector<Edge> neighbors;
    //     int destination;
    //     double weight;
    //     for(int k = 0; k < numOfNeighbors; k++) {
    //         file.read(reinterpret_cast<char*>(&destination), sizeof(int));
    //         file.read(reinterpret_cast<char*>(&weight), sizeof(int));
    //         neighbors.emplace_back(destination, weight);
    //     }
    //
    //     g[id] = neighbors;
    // }
    //
    // file.close();

}

// explicit template instantiations
template class FilterGraph<float>;
template class FilterGraph<int>;
template class FilterGraph<char>;