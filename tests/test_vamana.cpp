#include "../include/acutest.h"
#include "../src/main/cpp/utility/Graph/Graph.h"
#include <vector>
#include <iostream>

using namespace std;

void test_vamana() {
    // Δεδομένα δοκιμής
    vector<vector<int>> data = {
        {1, 2}, {2, 3}, {3, 4}, {5, 6}, {7, 8},
        {8, 9}, {0, 1}, {1, 0}, {3, 3}, {4, 4}
    };

    // Παράμετροι του αλγορίθμου Vamana
    int L = 5;       // Μέγεθος λίστας αναζήτησης
    int R = 3;       // Μέγιστος αριθμός γειτόνων
    int k = 3;       // Αριθμός γειτόνων για εύρεση
    double a = 1.5;  // Παράμετρος για το pruning

    // Δημιουργία του γράφου με τα δεδομένα και τις παραμέτρους
    Graph<int> graph(data, L, R, k, a);

    // Εκτέλεση του αλγορίθμου Vamana
    graph.vamana();

    // Έλεγχος ότι κάθε κόμβος έχει τουλάχιστον έναν γείτονα και όχι περισσότερους από R
    for (int nodeId : graph.getVerticesIds()) {
        const auto& neighbors = graph.getNeighbors(nodeId);
        TEST_CHECK(!neighbors.empty());
        TEST_CHECK(neighbors.size() <= R);
    }

    // Επαλήθευση των αποστάσεων των γειτόνων με περιθώριο σφάλματος λόγω κινητής υποδιαστολής
    const double epsilon = 1e-6;
    for (int nodeId : graph.getVerticesIds()) {
        const auto& neighbors = graph.getNeighbors(nodeId);
        for (const Edge& edge : neighbors) {
            int neighborId = edge.getDestination();
            double dist = edge.getWeight();
            double expectedDist = Graph<int>::euclideanDistance(
                graph.getVertex(nodeId), graph.getVertex(neighborId)
            );
            TEST_CHECK(abs(dist - expectedDist) < epsilon);
        }
    }

//    // Έλεγχος για μοναδικούς γείτονες (χωρίς διπλότυπα)
//    for (int nodeId : graph.getVerticesIds()) {
//        const auto& neighbors = graph.getNeighbors(nodeId);
//        set<int> neighborIds;
//        for (const Edge& edge : neighbors) {
//            neighborIds.insert(edge.getDestination());
//        }
//        TEST_CHECK(neighbors.size() == neighborIds.size());
//    }
}

TEST_LIST = {
    { "Vamana Algorithm", test_vamana },
    { NULL, NULL }
};
