//
// Created by mkorres on 11/11/2024.
//

#include "../FilterGraph/FilterGraph.h"

#include <fstream>

template <typename T>
FilterGraph<T>::FilterGraph(){}

template <typename T>
FilterGraph<T>::FilterGraph(vector<DataPoint<T>> dataPoints, const int L,const int R,const int k,double a, int tau, const int numOfThreads):numOfThreads(numOfThreads),numOfDatapoints(0),L(L),R(R),k(k),a(a),tau(tau){
    numOfDatapoints = dataPoints.size();
    set<int> filtersSet;
    for(unsigned int i = 0; i < numOfDatapoints ; i++ ) {
        vertexMap[dataPoints[i].id] = dataPoints[i];
        filtersSet.insert(dataPoints[i].C);
    }

    filters = vector(filtersSet.begin(), filtersSet.end());
    numOfFilters = filters.size();

    ids = vector<int>(numOfDatapoints);
    int i = 0;
    for (const auto& [id, _] : vertexMap){ ids[i] = id; i++;}
}

template <typename T>
void FilterGraph<T>::initializeRandomEdges(){
    // TIMER_BLOCK("Graph initialization",
        for(const auto& [id, _] : vertexMap) {
            vector<Edge> neighbors = randomNeighbors(id);
            g.insert({id,neighbors});
        }
    // )
}

template <typename T>
vector<Edge> FilterGraph<T>::randomNeighbors(int pId) {
    vector<Edge> neighbors;
    auto p = vertexMap[pId].vec;
    set<int> added;
    const unsigned int n = (numOfDatapoints >= R)? R : numOfDatapoints;

    vector<int> givenIds = getVerticesIds();

    for (unsigned int i = 0; i < n; ++i) {

        int randomId = getRandomId(givenIds);
        added.insert(randomId);

        auto randVec = vertexMap[randomId].vec;
        neighbors.push_back(Edge(randomId, euclideanDistance(p, randVec)));
    }


    return neighbors;
}


// Μειώνεται η πολυπλοκότητα από O(n^2) σε O(m^2), όπου m είναι το μέγεθος του δείγματος (m << n).
// Το δείγμα θα πρέπει να είναι περίπου το 10% του συνολικού αριθμού των κόμβων
template <typename T>
int FilterGraph<T>::medoid() {
    const int sample_size = numOfDatapoints/10 + 1;

    set<int> added;

    vector<int> givenIds = getVerticesIds();

    for (int i = 0; i < sample_size; ++i) {
        int randomId = getRandomId(givenIds);
        added.insert(randomId);
    }


    int medoid_id = -1;
    double min_total_distance = numeric_limits<double>::infinity();

    // Υπολογίζουμε το medoid στο δείγμα
    for (int id_i : added) {
        double total_distance = 0.0;
        for (int id_j : added) {
            if (id_i != id_j) {
                total_distance += euclideanDistance(vertexMap[id_i].vec, vertexMap[id_j].vec);
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
map<int, int> FilterGraph<T>::findMedoid() {
    // Δημιουργία των χάρτων M και medoidCount
    map<int, int> M;             // Χάρτης που αντιστοιχίζει κάθε φίλτρο στο medoid του
    map<int, int> medoidCount;   // Χάρτης που μετράει πόσες φορές κάθε σημείο είναι medoid

    // Αρχικοποίηση όλων των σημείων στο medoidCount με μηδενικές τιμές
    for (const auto& [id, dataPoint] : vertexMap) {
        medoidCount[id] = 0; // Όλα τα IDs ξεκινούν με μετρητή 0
    }

    // Δημιουργία ενός χάρτη που αντιστοιχίζει φίλτρα στα αντίστοιχα σημεία τους (P_f)
    map<int, vector<int>> filterToPoints;
    for (const auto& [id, dataPoint] : vertexMap) {
        filterToPoints[dataPoint.C].push_back(id);
    }

    // Επεξεργασία κάθε φίλτρου f στον χάρτη filterToPoints
    for (const auto& [f, P_f_cost] : filterToPoints) {
        // P_f: Συλλογή όλων των σημείων δεδομένων που σχετίζοντα με το φίλτρο f
        vector<int> P_f = P_f_cost;

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
    set<int> V;

    for (auto s : S) {
        l.insert({s,euclideanDistance(q,vertexMap[s].vec)});
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
}

template <typename T>
void FilterGraph<T>::filteredVamana() {
    // Αρχικοποίηση κενού συνόλου γειτόνων
    for (int x : ids) {
        g[x] = vector<Edge>();
    }

    // Προσθήκη τυχαίων ακμών
    for (size_t i = 0; i < ids.size(); ++i) {
        int x = ids[i];
        for (unsigned int j = 0; j < R; ++j) {
            int randomNeighbor = ids[rand() % ids.size()];
            if (randomNeighbor != x) {
                addEdge(x, randomNeighbor, euclideanDistance(vertexMap[x].vec, vertexMap[randomNeighbor].vec));
            }
        }
    }

    // Δημιουργία τυχαίων κόμβων εκκίνησης
    vector<int> randomStartNodes;
    int numStartNodes = std::min(static_cast<int>(ids.size()), 10); // π.χ., μέχρι 10 start nodes
    for (int i = 0; i < numStartNodes; ++i) {
        randomStartNodes.push_back(ids[rand() % ids.size()]);
    }

    set<int> done;
    vector<int> givenIds = getVerticesIds();
    Utils<int>::shuffle(givenIds);

    for(int x : givenIds){
        if (numOfDatapoints > 10 && !done.empty() && done.size() % (numOfDatapoints / 10) == 0) {
            printf("completed %d %% ... \n", static_cast<int>(done.size() * 100 / numOfDatapoints));
        }

        DataPoint<T>& dp = vertexMap[x];

        // Χρήση τυχαίων κόμβων εκκίνησης
        const auto& [_, Vx] = filteredGreedySearch(randomStartNodes, dp.vec, k, L, dp.C);


        g[x] = filteredRobustPrune(x, Vx, a, R);

        for (const Edge& e : g[x]) { // add reverse edges
            int dest = e.destination;
            int weight = e.weight;

            addEdge(dest, x, weight);

            // if dest out-degree violates R constraint => prune
            if (g[dest].size() > R)
                g[dest] = filteredRobustPrune(dest, edgesToVertices(g[dest]), a, R);

        }
        done.insert(x);
    }
    printf("completed 100 %% ... \n");
}

template <typename T>
map<int,int> FilterGraph<T>::getStartNodes(){
    map<int,int> S;
    for (int f : filters) {
        for (const auto& [id, dp] : vertexMap) { // assign as starting node the 1st of the corresponding filter class
            if (dp.C == f) {
                S[f] = id;
                break;
            }
        }
    }
    return S;
}

template <typename T>
void FilterGraph<T>::stitchedVamana() {

    vector<FilterGraph> graphs(numOfFilters);

    auto nt = this->numOfThreads;
    auto nf = this->numOfFilters;
    auto fs = this->filters;
    auto vmap = this->vertexMap;

    # pragma omp parallel for if(numOfThreads > 0) num_threads(nt) shared(nf,fs,vmap,graphs)
    for (int i = 0; i < nf;i++) {

        int f = fs[i];

        vector<DataPoint<T>> fvertices;
        for (const auto& [id, dp] : vmap) if (dp.C == f) fvertices.push_back(dp);

        if(fvertices.empty()) cout << "No filter graph found" << endl;

        graphs[f] = FilterGraph<T>(fvertices,L,(R+4)/4,k,a,tau,nt); // Rsmall = floor(R/4) + 1

        if(graphs[f].numOfDatapoints > 1) {
            // printf("-------- Vamana on graph[f],f = %d\n",f);
            graphs[f].vamana();
        }
        else {
            // printf("-------- Single-node graph[f],f = %d\n",f);
        }

        DIVIDER

    }

    for (int i = 0; i < numOfFilters;i++) {
        int f = filters[i];
        for (const auto& [id, edges] : graphs[f].g) {
            for (const auto& edge : edges) {
                addEdge(id, edge.destination, edge.weight);
            }
        }
    }

    for (const auto& [id, dp] : vertexMap) {
        filteredRobustPrune(id, edgesToVertices(g[id]),a,R);
    }

}

template<typename T>
pair<vector<int>,vector<int>> FilterGraph<T>::greedySearch(int s, const vector<T>& q, const int k, int L) {

    VamanaContainer l(L); l.insert({s,euclideanDistance(q,vertexMap[s].vec)});      // Σύνολο αναζήτησης
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


template <typename T>
void FilterGraph<T>::vamana(){
    cout << "vamana started" << endl;

    initializeRandomEdges();

    const int s = medoid();

    cout << "medoid calculated" << endl;

    set<int> done;
    vector<int> givenIds = getVerticesIds();
    Utils<int>::shuffle(givenIds);

    for(int x : givenIds){
        if (numOfDatapoints > 10 && !done.empty() && done.size() % (numOfDatapoints / 10) == 0) {
            printf("completed %d %% ... \n", static_cast<int>(done.size() * 100 / numOfDatapoints));
        }

        const auto& [l,Vx] = greedySearch(s,vertexMap[x].vec,k,L);

        g[x] = robustPrune(x,Vx,a,R);

        vector<int> xNeighbors =  edgesToVertices(g[x]);

        for(auto y: xNeighbors){
            if(g[y].size() + 1 > R){
                vector<int> vertices = edgesToVertices(g[y]);
                vertices.push_back(x);
                g[y] = robustPrune(y,vertices,a,R);
            }
            else{
                g[y].push_back(Edge(x,euclideanDistance(vertexMap[y].vec,vertexMap[x].vec)));
            }
        }
        done.insert(x);
    }
    printf("completed 100 %% ... \n");
}

template <typename T>
int FilterGraph<T>::getRandomId(vector<int>& givenIds) {
    if (givenIds.empty()) throw std::runtime_error("No elements in givenIds to select from");
    const int i = Utils<int>::random(0,static_cast<int>(givenIds.size() - 1));
    const auto id = givenIds[i];
    givenIds.erase(givenIds.begin() + i);
    return id;
}

template <typename T>
vector<Edge> FilterGraph<T>::robustPrune(int p, const vector<int> &V, double a, const unsigned int R) {
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
        int p_star = argminDist(vertexMap[p].vec, candidateNeighbors); // Χρήση της συνάρτησής σου

        // Προσθήκη του p_star στους νέους γείτονες
        N_out.push_back(Edge(p_star, euclideanDistance(vertexMap[p].vec, vertexMap[p_star].vec)));

        // Αφαίρεση του p_star από τους υποψήφιους
        candidateNeighbors.erase(std::remove(candidateNeighbors.begin(), candidateNeighbors.end(), p_star), candidateNeighbors.end());


        // Αν το πλήθος των νέων γειτόνων φτάσει το όριο R, σταματάμε
        if (N_out.size() == R) break;


        // Κλαδεύουμε τους υπόλοιπους υποψήφιους γείτονες
        for (auto it = candidateNeighbors.begin(); it != candidateNeighbors.end();) {
            if (a * euclideanDistance(vertexMap[p_star].vec, vertexMap[*it].vec) <= euclideanDistance(vertexMap[p].vec, vertexMap[*it].vec)) {
                it = candidateNeighbors.erase(it); // Αφαίρεση των γειτόνων που δεν πληρούν τα κριτήρια
            } else {
                ++it;
            }
        }
    }

    return N_out; // Επιστροφή των επιλεγμένων γειτόνων
}


template <typename T>
vector<Edge> FilterGraph<T>::filteredRobustPrune(int p, const vector<int> &V, double a,unsigned int R) {
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
        int p_star = argminDist(vertexMap[p].vec, candidateNeighbors);

        // Προσθήκη του p_star στους νέους γείτονες
        N_out.push_back(Edge(p_star, euclideanDistance(vertexMap[p].vec, vertexMap[p_star].vec)));

        // Αφαίρεση του p_star από τους υποψήφιους
        candidateNeighbors.erase(std::remove(candidateNeighbors.begin(), candidateNeighbors.end(), p_star), candidateNeighbors.end());


        // Αν το πλήθος των νέων γειτόνων φτάσει το όριο R, σταματάμε
        if (N_out.size() == R) break;


        // Κλαδεύουμε τους υπόλοιπους υποψήφιους γείτονες
        for (auto it = candidateNeighbors.begin(); it != candidateNeighbors.end();) {
            if (vertexMap[*it].C != vertexMap[p_star].C && vertexMap[p].C != vertexMap[p_star].C) {
                ++it;
                continue;
            }
            if (a * euclideanDistance(vertexMap[p_star].vec, vertexMap[*it].vec) <= euclideanDistance(vertexMap[p].vec, vertexMap[*it].vec)) {
                it = candidateNeighbors.erase(it); // Αφαίρεση των γειτόνων που δεν πληρούν τα κριτήρια
            } else {
                ++it;
            }
        }
    }

    return N_out; // Επιστροφή των επιλεγμένων γειτόνων
}



////////


template <typename T>
void FilterGraph<T>::addEdge(int src, int dest,float dist){
    vector<Edge>& srcNeighbors = g[src];
    srcNeighbors.push_back(Edge(dest,dist));
}

template <typename T>
void FilterGraph<T>::addVertex(DataPoint<T> vertex){
    vertexMap.insert({vertex.id,vertex});
    g.insert({vertex.id,vector<Edge>()});
    numOfDatapoints++;
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
    const unsigned int dim = v1.size();
    float dist = 0;

    # pragma omp parallel for if(numOfThreads > 0) num_threads(numOfThreads) shared(v1,v2,dim) reduction(+:dist)
    for(unsigned int j = 0; j < dim; j++) {
        dist += (v2[j] - v1[j]) * (v2[j] - v1[j]);
    }

    return sqrt(dist);
}

template<typename T>
double FilterGraph<T>::equals(const vector<T>& v1, vector<T>& v2) {

    const int dim = v2.size();

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

    return static_cast<double>(dim - misses)/dim;
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
void FilterGraph<T>::printVectorNeighbors(vector<Edge>& neighbors,ostream& out) {

    out << "Neighbors:" << endl;
    if(!neighbors.empty()) {
        for(Edge neighbor : neighbors){
            out << "[" << neighbor.destination << "] = ";
            Utils<T> :: printVec(vertexMap[neighbor.destination].vec);
        }
    }
    else {
        out << "No neighbors" << endl;
    }
}

template<typename T>
DataPoint<T> FilterGraph<T>::getVertex(unsigned int id) {
    if(id >= vertexMap.size()) return DataPoint<T>();
    return vertexMap[id];
}

// import-export graph
template <typename T>
void FilterGraph<T>::exportGraph(const string& filename) {

    const string graphFilePath(RESOURCES_P + filename);

    ofstream file(graphFilePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // persist graph metadata
    file.write(reinterpret_cast<const char*>(&numOfDatapoints), sizeof(int));
    file.write(reinterpret_cast<const char*>(&L), sizeof(int));
    file.write(reinterpret_cast<const char*>(&R), sizeof(int));
    file.write(reinterpret_cast<const char*>(&k), sizeof(int));
    file.write(reinterpret_cast<const char*>(&a), sizeof(double));
    const int d = vertexMap[0].vec.size();
    file.write(reinterpret_cast<const char*>(&d), sizeof(int)); // dimension of a datapoint => same for all

    // persist vector type size
    size_t typeSize = sizeof(T);
    file.write(reinterpret_cast<const char*>(&typeSize), sizeof(size_t));

    size_t numOfNeighbors;

    // persist vectors
    for(const auto& [_, datapoint] : vertexMap) {

        file.write(reinterpret_cast<const char*>(&datapoint.id), sizeof(int));
        file.write(reinterpret_cast<const char*>(&datapoint.C), sizeof(int));
        file.write(reinterpret_cast<const char*>(&datapoint.T), sizeof(int));

        for(auto& xi : datapoint.vec) {
            file.write(reinterpret_cast<const char*>(&xi), sizeof(typeSize));
        }

        // persist num of neighbors
        numOfNeighbors = g[datapoint.id].size();
        file.write(reinterpret_cast<const char*>(&numOfNeighbors), sizeof(size_t));

        // persist neighbors
        auto& neighbors = g[datapoint.id];
        for(auto& n : neighbors) {
            file.write(reinterpret_cast<const char*>(&n.destination), sizeof(int));
            file.write(reinterpret_cast<const char*>(&n.weight), sizeof(double));
        }
    }

    file.close();

}

template <typename T>
void FilterGraph<T>::importGraph(const string& filename) {

    const string graphFilePath(RESOURCES_P + filename);

    ifstream file(graphFilePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // fetch graph metadata
    file.read(reinterpret_cast<char*>(&numOfDatapoints), sizeof(int));
    file.read(reinterpret_cast<char*>(&L), sizeof(int));
    file.read(reinterpret_cast<char*>(&R), sizeof(int));
    file.read(reinterpret_cast<char*>(&k), sizeof(int));
    file.read(reinterpret_cast<char*>(&a), sizeof(double));
    int d = 0;
    file.read(reinterpret_cast<char*>(&d), sizeof(int)); // dimension of a datapoint => same for all


    // fetch vector type size
    size_t typeSize;
    file.read(reinterpret_cast<char*>(&typeSize), sizeof(size_t));

    size_t numOfNeighbors = 0;

    // fetch vectors
    for(unsigned int i = 0; i < numOfDatapoints; i++) {
        DataPoint<T> datapoint = DataPoint<T>();

        file.read(reinterpret_cast<char*>(&datapoint.id), sizeof(int));
        file.read(reinterpret_cast<char*>(&datapoint.C), sizeof(int));
        file.read(reinterpret_cast<char*>(&datapoint.T), sizeof(int));

        T xi;
        for(int j = 0; j < d; j++) {
            file.read(reinterpret_cast<char*>(&xi), sizeof(typeSize));
            datapoint.vec.push_back(xi);
        }

        vertexMap.insert({datapoint.id, datapoint});

        // fetch num of neighbors
        file.read(reinterpret_cast<char*>(&numOfNeighbors), sizeof(size_t));

        // fetch neighbors
        vector<Edge> neighbors;
        int destination;
        double weight;
        for(unsigned int k = 0; k < numOfNeighbors; k++) {
            file.read(reinterpret_cast<char*>(&destination), sizeof(int));
            file.read(reinterpret_cast<char*>(&weight), sizeof(double));
            neighbors.push_back(Edge(destination, weight));
        }

        g[datapoint.id] = neighbors;
    }

    file.close();

}

// explicit template instantiations
template class FilterGraph<float>;
template class FilterGraph<int>;
template class FilterGraph<char>;