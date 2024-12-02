#include "Graph.h"

template <typename T>
Graph<T>::Graph(vector<vector<T>> vecs, const int L,const int R,const int k,double a):AUTO_INCREMENT(0),R(R),k(k),a(a),L(L),d((vecs.size() > 0)?static_cast<int>(vecs[0].size()):0){
    // TODO: Revisit d initialization
    for(int i = 0; i < vecs.size() ; i++ ) {
        vertexMap.insert({AUTO_INCREMENT,vecs[i]});
        AUTO_INCREMENT++;
    }
}

template <typename T>
void Graph<T>::vamana(){
    cout << "vamana started" << endl;

    initializeRandomEdges();

    const int s = medoid();
    auto sigma = Utils<T>::shuffle;

    cout << "medoid calculated" << endl;

    vector<int> V;
    int x;
    set<int> done;
    while(done.size() != vertexMap.size()){
        (!done.empty() && done.size() % 1000 == 0) && printf("completed %d %% ... \n",static_cast<int>(done.size()/100));

        while (done.find(x = Utils<int>::random(0,AUTO_INCREMENT - 1)) != done.end()) {}
        done.insert(x);

        const auto& [l,V] = greedySearch(s,vertexMap[x],k,L);

        vector<int> neighborIds = getVerticesIds();
        g[x] = robustPrune(x,V,a,R);

        vector<int> xNeighbors =  edgesToVertices(g[x]);

        for(auto y: xNeighbors){
            if(g[y].size() + 1 > R){
                vector<int> V = edgesToVertices(g[y]);
                V.push_back(x);
                g[y] = robustPrune(y,V,a,R);
            }
            else{
                g[y].push_back(Edge(x,euclideanDistance(vertexMap[y],vertexMap[x])));
            }
        }
    }
    printf("completed %d %%\n",static_cast<int>(done.size()/100));
}

template <typename T>
void Graph<T>::initializeRandomEdges(){
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

    int randomId = Utils<int>::random(0,AUTO_INCREMENT - 1);

    set<int> randomIds;

    for (int i = 0; i < sample_size; ++i) {
        while (randomIds.find(randomId) != randomIds.end()) {
            randomId = Utils<int>::random(0,AUTO_INCREMENT - 1);
        }
        randomIds.insert(randomId);
    }

    int medoid_id = -1;
    double min_total_distance = numeric_limits<double>::infinity();

    // Υπολογίζουμε το medoid στο δείγμα
    for (int id_i : randomIds) {
        double total_distance = 0.0;
        for (int id_j : randomIds) {
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
        remove_if(srcNeighbors.begin(), srcNeighbors.end(),[&](const Edge& e) { return e.destination == dest; }),
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

    VamanaContainer l(L); l.insert({s,euclideanDistance(q,vertexMap[s])});      // Σύνολο αναζήτησης
    set<int> V;         // Σύνολο επισκεφθέντων κόμβων
    set<int> diff = setDiff(l, V);

    while(!diff.empty()){
        int pStar = argmindist(q,diff);   // Βρίσκουμε το p* (τον πιο κοντινό κόμβο που δεν έχει επισκεφτεί ακόμα)

        vector<Edge> neighbors = g[pStar];  // Βρίσκουμε τους γείτονες του p*

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
}

template<typename T>
set<int> Graph<T>::setDiff(VamanaContainer& A,set<int>& B){
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
vector<int> Graph<T>::edgesToVertices(vector<Edge> edges) {
    vector<int> v;
    // Use a lambda to extract destinations and push to v
    transform(edges.begin(), edges.end(), back_inserter(v), [](const Edge& e) {
        return e.destination; // Return the destination to be inserted into v
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
    vector<Edge> pOut = getNeighbors(p);
    for (Edge e : pOut) {
        if (find(candidateNeighbors.begin(), candidateNeighbors.end(), e.destination) == candidateNeighbors.end()) {
            candidateNeighbors.push_back(e.destination);
        }
    }
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
        if (N_out.size() == R) break;


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

            out << "[" << neighbor.destination << "] = ";
            for(int j = 0; j < vertexMap[neighbor.destination].size(); j++) {
                out << vertexMap[neighbor.destination][j] << " ";
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
vector<T> Graph<T>::getVertex(int id) {
    if(id >= vertexMap.size()) return vector<T>();
    return vertexMap[id];
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

// For testing
template <typename T>
int Graph<T>::getTotalVertices() const {
    return vertexMap.size();
}

template <typename T>
const vector<T>& Graph<T>::getVertexData(int id) const {
    auto it = vertexMap.find(id);
    if (it != vertexMap.end()) {
        return it->second;
    } else {
        throw std::out_of_range("Vertex ID not found");
    }
}

template <typename T>
const vector<Edge>& Graph<T>::getEdges(int id) const {
    auto it = g.find(id);
    if (it != g.end()) {
        return it->second;
    } else {
        static const vector<Edge> empty;
        return empty;
    }
}

template <typename T>
int Graph<T>::getTotalEdges() const {
    int count = 0;
    for (const auto& pair : g) {
        count += pair.second.size();
    }
    return count;
}

// import-export graph
template <typename T>
void Graph<T>::exportGraph() {

    const string filename = "graph.bin";
    const string graphFilePath(RESOURCES_P + filename);

    ofstream file(graphFilePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // persist graph metadata
    file.write(reinterpret_cast<const char*>(&AUTO_INCREMENT), sizeof(int));
    file.write(reinterpret_cast<const char*>(&L), sizeof(int));
    file.write(reinterpret_cast<const char*>(&R), sizeof(int));
    file.write(reinterpret_cast<const char*>(&k), sizeof(int));
    file.write(reinterpret_cast<const char*>(&d), sizeof(int));
    file.write(reinterpret_cast<const char*>(&a), sizeof(double));

    // persist vector type size
    size_t typeSize = sizeof(T);
    file.write(reinterpret_cast<const char*>(&typeSize), sizeof(size_t));

    size_t numOfNeighbors;

    // persist vectors
    for(const auto& [id, vector] : vertexMap) {
        file.write(reinterpret_cast<const char*>(&id), sizeof(int));
        for(auto& xi : vector) {
            file.write(reinterpret_cast<const char*>(&xi), sizeof(typeSize));
        }

        // persist num of neighbors
        numOfNeighbors = g[id].size();
        file.write(reinterpret_cast<const char*>(&numOfNeighbors), sizeof(size_t));

        // persist neighbors
        auto& neighbors = g[id];
        for(auto& n : neighbors) {
            file.write(reinterpret_cast<const char*>(&n.destination), sizeof(int));
            file.write(reinterpret_cast<const char*>(&n.weight), sizeof(int));
        }
    }

    file.close();

}

template <typename T>
void Graph<T>::importGraph() {

    const string filename = "graph.bin";
    const string graphFilePath(RESOURCES_P + filename);

    ifstream file(graphFilePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // fetch graph metadata
    file.read(reinterpret_cast<char*>(&AUTO_INCREMENT), sizeof(int));
    file.read(reinterpret_cast<char*>(&L), sizeof(int));
    file.read(reinterpret_cast<char*>(&R), sizeof(int));
    file.read(reinterpret_cast<char*>(&k), sizeof(int));
    file.read(reinterpret_cast<char*>(&d), sizeof(int));
    file.read(reinterpret_cast<char*>(&a), sizeof(double));

    // fetch vector type size
    size_t typeSize;;
    file.read(reinterpret_cast<char*>(&typeSize), sizeof(size_t));

    size_t numOfNeighbors;

    // fetch vectors
    for(int i = 0; i < AUTO_INCREMENT; i++) {
        int id;
        vector<T> vec;

        file.read(reinterpret_cast<char*>(&id), sizeof(int));
        T xi;
        for(int j = 0; j < d; j++) {
            file.read(reinterpret_cast<char*>(&xi), sizeof(typeSize));
            vec.push_back(xi);
        }

        vertexMap[id] = vec;

        // fetch num of neighbors
        file.read(reinterpret_cast<char*>(&numOfNeighbors), sizeof(size_t));

        // fetch neighbors
        vector<Edge> neighbors;
        int destination;
        double weight;
        for(int k = 0; k < numOfNeighbors; k++) {
            file.read(reinterpret_cast<char*>(&destination), sizeof(int));
            file.read(reinterpret_cast<char*>(&weight), sizeof(int));
            neighbors.emplace_back(destination, weight);
        }

        g[id] = neighbors;
    }

    file.close();

}

// explicit template instantiations
template class Graph<float>;
template class Graph<int>;
template class Graph<char>;