#ifndef GRAPH_CPP
#define GRAPH_CPP

#include "Graph.h"
#include "../Utils/Utils.h"

template <typename T>
Graph<T>::Graph(const vector<vector<T>>& vecs, int R, int k, double a, int L)
    : AUTO_INCREMENT(0), R(R), k(k), a(a), L(L) {
    for (const auto& vec : vecs) {
        addVertex(vec);  // Προσθέτουμε κάθε σημείο στον vertexMap
    }
    //this->printGraph(std::cout);
    this->vamanaIndexing(vecs, L, R, a);
    //this->printGraph(std::cout);
}

template <typename T>
void Graph<T>::addVertex(const vector<T>& vertex) {
    vertexMap[AUTO_INCREMENT++] = vertex;
}

template <typename T>
void Graph<T>::addEdge(int src, int dest, float dist) {
    if (vertexMap.find(src) == vertexMap.end() || vertexMap.find(dest) == vertexMap.end()) {
        throw std::out_of_range("Source or destination vertex does not exist");
    }
    // Έλεγχος για διπλή ακμή
    auto& edges = g[src];
    auto it = std::find_if(edges.begin(), edges.end(),
                           [&](const Edge& edge) { return edge.getDestination() == dest; });
    if (it == edges.end()) {
        g[src].emplace_back(Edge(dest, dist)); // Προσθέτει μια ακμή από το src στο dest
    }
}

template <typename T>
vector<int> Graph<T>::getOutNeighbors(int vertex) {
    vector<int> neighbors;
    for (const auto& edge : g[vertex]) {
        neighbors.push_back(edge.getDestination());
    }
    return neighbors;
}

template <typename T>
void Graph<T>::initializeRandomGraph(int R) {
    vector<int> allVertices;        // Όλα τα IDs των κορυφών
    for (const auto& node : vertexMap) {
        allVertices.push_back(node.first);
    }

    // Για κάθε κορυφή στο γράφημα
    for (const auto& node : vertexMap) {
        int vertex = node.first;
        vector<int> possibleNeighbors = allVertices;        // Δημιουργούμε ένα αντίγραφο των κορυφών και αφαιρούμε την τρέχουσα κορυφή
        possibleNeighbors.erase(remove(possibleNeighbors.begin(), possibleNeighbors.end(), vertex), possibleNeighbors.end());

        // Αν ο αριθμός των δυνατών γειτόνων είναι μικρότερος από R, προσαρμόζουμε το R
        int numNeighbors = min(R, static_cast<int>(possibleNeighbors.size()));

        // Ανακατεύουμε τυχαία τους δυνατούς γείτονες
        random_shuffle(possibleNeighbors.begin(), possibleNeighbors.end());

        // Επιλέγουμε τους πρώτους numNeighbors γείτονες
        for (int i = 0; i < numNeighbors; ++i) {
            int randomNeighbor = possibleNeighbors[i];
            float dist = euclideanDistance(vertexMap[vertex], vertexMap[randomNeighbor]);
            addEdge(vertex, randomNeighbor, dist);
        }
    }
}

template <typename T>
int Graph<T>::findMedoid() {
    int medoid_id = -1;
    double min_total_distance = numeric_limits<double>::infinity();

    // Διατρέχουμε όλα τα σημεία για να υπολογίσουμε το medoid
    for (const auto& [id_i, vec_i] : vertexMap) {
        double total_distance = 0.0;
        for (const auto& [id_j, vec_j] : vertexMap) {
            if (id_i != id_j) {
                total_distance += euclideanDistance(vec_i, vec_j);
            }
        }
        if (total_distance < min_total_distance) {
            min_total_distance = total_distance;
            medoid_id = id_i;
        }
    }

    return medoid_id;
}

// Μειώνεται η πολυπλοκότητα από O(n^2) σε O(m^2), όπου m είναι το μέγεθος του δείγματος (m << n).
// Το δείγμα θα πρέπει να είναι περίπου το 10% του συνολικού αριθμού των κόμβων
template <typename T>
int Graph<T>::findMedoidSample(int sample_size) {
    int n = vertexMap.size();
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
vector<int> Graph<T>::generateRandomPermutation(int n) {
    vector<int> sigma(n);
    iota(sigma.begin(), sigma.end(), 0); // Γεμίζουμε με 0..n-1
    random_shuffle(sigma.begin(), sigma.end()); // Τυχαία διάταξη
    return sigma;
}

template<typename T>
double Graph<T>::euclideanDistance(const vector<T>& v1,const vector<T>& v2) {
    int d = v1.size();
    float dist = 0;
    for(int j = 0; j < d; j++) {
        dist += (v2[j] - v1[j]) * (v2[j] - v1[j]);
    }
    return sqrt(dist);
}

template <typename T>
void Graph<T>::setOutNeighbors(int node, const std::vector<int>& neighbors) {
    // Καθαρίζουμε τους υπάρχοντες γείτονες του κόμβου
    g[node].clear();

    // Για κάθε γείτονα στη νέα λίστα
    for (int neighbor : neighbors) {
        // Υπολογίζουμε την απόσταση μεταξύ του κόμβου και του γείτονά του
        double dist = euclideanDistance(vertexMap[node], vertexMap[neighbor]);

        // Δημιουργούμε μια νέα ακμή από τον κόμβο προς τον γείτονά του
        Edge edge(neighbor, dist);

        // Προσθέτουμε την ακμή στη λίστα των εξερχόμενων ακμών του κόμβου
        g[node].push_back(edge);
    }
}

template <typename T>
void Graph<T>::addOutNeighbor(int node, int neighbor) {
    // Έλεγχος αν ο neighbor υπάρχει ήδη
    auto it = std::find_if(g[node].begin(), g[node].end(),
                           [&](const Edge& edge) { return edge.getDestination() == neighbor; });
    if (it == g[node].end()) {
        // Υπολογίζουμε την απόσταση και προσθέτουμε τον neighbor
        double dist = euclideanDistance(vertexMap[node], vertexMap[neighbor]);
        Edge edge(neighbor, dist);
        g[node].push_back(edge);
    }
}





// P: Βάση δεδομένων με n σημεία, το κάθε σημείο έχει συντεταγμένες x_i.
// L: Παράμετρος για το GreedySearch που καθορίζει το μέγεθος της λίστας αναζήτησης.
// R: Το μέγιστο πλήθος των εξερχόμενων γειτόνων για κάθε κόμβο.
// s: Το medoid του συνόλου δεδομένων (το σημείο με τη μικρότερη μέση απόσταση από όλα τα υπόλοιπα).
// σ: Μια τυχαία διάταξη των σημείων.
// Nout: Τα εξερχόμενα γειτονικά σημεία για κάθε κόμβο.

template <typename T>
void Graph<T>::vamanaIndexing(const vector<vector<T>>& P, int L, int R, double a) {
    //cout << "vamana started" << endl;

    // Αρχικοποίηση του G ως τυχαίο R-regular γράφημα
    this->initializeRandomGraph(R);
    //cout << "random edges initialized" << endl;

    // Υπολογισμός του medoid του συνόλου P
    int s;
    if (P.size() < 1000 ) {
         s = findMedoid();
    }else {
        s = findMedoidSample(P.size()/10);  // Το δείγμα θα πρέπει να είναι περίπου το 10% του συνολικού αριθμού των κόμβων
    }
    //cout << "Medoid found: " << s << endl;

    // Δημιουργία τυχαίας διάταξης των σημείων 1..n
    vector<int> sigma = generateRandomPermutation(P.size());
    //cout << "Random permutation generated" << endl;

    // Για κάθε σημείο στη διάταξη
    for (int i = 0; i < P.size(); i++) {
        int sigma_i = sigma[i];

        // Εκτελούμε GreedySearch για το σ(i)
        auto [L_set, V] = greedySearch(s, P[sigma_i], 1, L);

        // Εκτελούμε RobustPrune για να ενημερώσουμε τους γείτονες του σ(i)
        vector<int> N_out = robustPrune(sigma_i, vector<int>(V.begin(), V.end()), a, R);

        // Ενημερώνουμε τους εξερχόμενους γείτονες του σ(i)
        setOutNeighbors(sigma_i, N_out);

        // Για κάθε γείτονα j στο Nout(σ(i))
        for (int j : getOutNeighbors(sigma_i)) {
            // Αν το πλήθος των γειτόνων του j υπερβαίνει το R
            if (getOutNeighbors(j).size() + 1 > R) {
                // Εκτελούμε RobustPrune για να ενημερώσουμε τους γείτονες του j
                vector<int> combinedNeighbors = getOutNeighbors(j);
                combinedNeighbors.push_back(sigma_i);
                vector<int> prunedNeighbors = robustPrune(j, combinedNeighbors, a, R);
                setOutNeighbors(j, prunedNeighbors);
            } else {
                // Ενημερώνουμε το σύνολο των γειτόνων του j
                addOutNeighbor(j, sigma_i);
            }
        }
    }
}


// επιστρέφει:      1) Τους k πιο κοντινούς γείτονες.
//                  2) Το σύνολο των κόμβων που έχουν επισκεφθεί.
// δέχεται:   1) s -> Το αρχικό σημείο (start node).
//            2) x_q -> Το σημείο ερώτημα (query point), δηλαδή το σημείο για το οποίο ψάχνουμε τους κοντινότερους γείτονες.
//            3) k -> Το πλήθος των γειτόνων που θέλουμε να βρούμε (μέγεθος του αποτελέσματος).
//            4) L -> Το μέγεθος της λίστας αναζήτησης, το οποίο πρέπει να είναι τουλάχιστον ίσο με το k.
template <typename T>
pair<vector<int>, unordered_set<int>> Graph<T>::greedySearch(int s, const vector<T>& x_q, int k, int ef) {
    // Comparator για την ταξινόμηση με βάση την απόσταση
    auto cmp = [](const pair<double, int>& a, const pair<double, int>& b) {
        if (a.first != b.first)
            return a.first < b.first; // Ταξινόμηση σε αύξουσα σειρά απόστασης
        else
            return a.second < b.second; // Αν οι αποστάσεις είναι ίσες, ταξινόμηση με βάση το ID
    };

    // CandidateSet: Σύνολο υποψηφίων κόμβων με τις αποστάσεις τους
    set<pair<double, int>, decltype(cmp)> candidateSet(cmp);

    // VisitedSet V: Σύνολο των επισκεμμένων κόμβων
    unordered_set<int> visited;

    // Αρχικοποίηση του CandidateSet με τον αρχικό κόμβο s
    double dist_s = euclideanDistance(x_q, vertexMap[s]);
    candidateSet.insert({dist_s, s});

    while (true) {
        // Βρίσκουμε το p* από το CandidateSet \ V με την ελάχιστη απόσταση
        pair<double, int> p_star_pair = {numeric_limits<double>::infinity(), -1};
        for (const auto& pair : candidateSet) {
            int node_id = pair.second;
            if (visited.find(node_id) == visited.end()) {
                p_star_pair = pair;
                break;
            }
        }

        // Αν δεν υπάρχει μη επισκεμμένος κόμβος, σταματάμε
        if (p_star_pair.second == -1) {
            break;
        }

        double dist_p_star = p_star_pair.first;
        int p_star = p_star_pair.second;

        // Προσθέτουμε τον p_star στο σύνολο των επισκεμμένων κόμβων
        visited.insert(p_star);

        // Ενημερώνουμε το CandidateSet με τους γείτονες του p_star
        for (const Edge& edge : g[p_star]) {
            int neighbor_id = edge.getDestination();
            if (visited.find(neighbor_id) == visited.end()) {
                double dist_neighbor = euclideanDistance(x_q, vertexMap[neighbor_id]);
                candidateSet.insert({dist_neighbor, neighbor_id});
            }
        }

        // Αν το μέγεθος του CandidateSet υπερβαίνει το ef, διατηρούμε μόνο τους πλησιέστερους ef κόμβους
        while (candidateSet.size() > ef) {
            auto it = candidateSet.end();
            --it; // Τελευταίο στοιχείο (μεγαλύτερη απόσταση)
            candidateSet.erase(it);
        }
    }

    // Συλλέγουμε τους k πλησιέστερους κόμβους από το CandidateSet
    vector<int> result;
    for (const auto& pair : candidateSet) {
        int node_id = pair.second;
        result.push_back(node_id);
        if (result.size() == k) {
            break;
        }
    }

    return {result, visited};
}


// επιστρέφει μια λίστα που περιέχει τα id των γειτόνων που επιλέχθηκαν μετά τη διαδικασία του pruning
// δέχεται:     1) p -> Το σημείο για το οποίο εκτελείται το pruning (ο κόμβος του γράφου).
//              2) V -> Το σύνολο των υποψήφιων γειτόνων που θα εξεταστούν για το pruning.
//              3) a -> Ένα κατώφλι (threshold) για την απόσταση, με τιμή μεγαλύτερη ή ίση του 1. Χρησιμοποιείται για να κρίνει αν θα αφαιρεθεί κάποιος γείτονας.
//              4) R -> Το μέγιστο όριο γειτόνων που μπορούμε να διατηρήσουμε (degree bound).
template <typename T>
vector<int> Graph<T>::robustPrune(int p, const vector<int> &V, double a, int R) {
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
        N_out.push_back(p_star);
        candidateNeighbors.erase(minIt); // Αφαίρεση του από τους υποψήφιους

        // Αν το πλήθος των νέων γειτόνων φτάσει το όριο R, σταματάμε
        if (N_out.size() == R) {
            break;
        }

        double dist_p_pstar = euclideanDistance(vertexMap[p], vertexMap[p_star]);

        // Κλαδεύουμε τους υπόλοιπους υποψήφιους γείτονες
        for (auto it = candidateNeighbors.begin(); it != candidateNeighbors.end();) {
            double dist_p_it = euclideanDistance(vertexMap[p], vertexMap[*it]);
            if (a * dist_p_pstar <= dist_p_it) {
                it = candidateNeighbors.erase(it); // Αφαίρεση των γειτόνων που δεν πληρούν τα κριτήρια
            } else {
                ++it;
            }
        }
    }

    return N_out; // Επιστροφή των επιλεγμένων γειτόνων
}



template <typename T>
void Graph<T>::printGraph(ostream& out){
    out << "Graph with " << vertexMap.size() << " vertices:" << endl;
    for(const auto& pair : vertexMap){
        //printVector(pair);
        //const int id = pair.first;
        //printVectorNeighbors(g[id]);
        out << "Vertex " << pair.first << "\t";
        out << "Neighbors: ";
        vector<Edge> neighbors = g[pair.first];
        for(Edge neighbor : neighbors) {
            out <<neighbor.getDestination() << " ";
        }
        out << endl;
    }
}
// Δημιουργία αρχείου DOT για απεικόνιση με Graphviz
template <typename T>
void Graph<T>::generateDotFile(const string& filename) {
    ofstream file(filename);
    file << "digraph G {\n";  // Για κατευθυνόμενο γράφο, αν ο γράφος είναι ακατεύθυντος, χρησιμοποίησε "graph G"

    for (const auto& node : g) {
        int source = node.first;
        for (const auto& edge : node.second) {
            int destination = edge.getDestination();
            float weight = edge.getWeight();
            // Γράφουμε την ακμή στο αρχείο DOT
            file << "    " << source << " -> " << destination << " [label=\"" << weight << "\"];\n";
        }
    }

    file << "}\n";
    file.close();
    cout << "DOT file created: " << filename << endl;
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


#endif // GRAPH_CPP
