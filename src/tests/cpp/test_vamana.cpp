#include "../../../include/acutest.h"
#include "../../main/cpp/utility/FilterGraph/FilterGraph.h"
#include "../../main/cpp/utility/DataPoint/DataPoint.h"
#include <vector>
#include <iostream>

using namespace std;

void test_vamana() {
    // Δεδομένα δοκιμής
    DataPoint<int> data0(0,-1,-1,vector<int>({1, 2}));
    DataPoint<int> data1(1,-1,-1,vector<int>({2, 3}));
    DataPoint<int> data2(2,-1,-1,vector<int>({3, 4}));
    DataPoint<int> data3(3,-1,-1,vector<int>({5, 6}));
    DataPoint<int> data4(4,-1,-1,vector<int>({7, 8}));
    DataPoint<int> data5(5,-1,-1,vector<int>({8, 9}));
    DataPoint<int> data6(6,-1,-1,vector<int>({0, 1}));
    DataPoint<int> data7(7,-1,-1,vector<int>({1, 0}));
    DataPoint<int> data8(8,-1,-1,vector<int>({3, 3}));
    DataPoint<int> data9(9,-1,-1,vector<int>({4, 4}));

    vector<DataPoint<int>> data = {data0,data1,data2,data3,data4,data5,data6,data7,data8,data9};


    // Παράμετροι του αλγορίθμου Vamana
    int L = 5;       // Μέγεθος λίστας αναζήτησης
    unsigned int R = 3;       // Μέγιστος αριθμός γειτόνων
    int k = 3;       // Αριθμός γειτόνων για εύρεση
    double a = 1.5;  // Παράμετρος για το pruning

    // Δημιουργία του γράφου με τα δεδομένα και τις παραμέτρους
    FilterGraph<int> graph(data, L, R, k, a,-1);

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
            int neighborId = edge.destination;
            double dist = edge.weight;
            double expectedDist = FilterGraph<int>::euclideanDistance(graph.getVertex(nodeId).vec, graph.getVertex(neighborId).vec);
            TEST_CHECK(abs(dist - expectedDist) < epsilon);
        }
    }

//    // Έλεγχος για μοναδικούς γείτονες (χωρίς διπλότυπα)
//    for (int nodeId : graph.getVerticesIds()) {
//        const auto& neighbors = graph.getNeighbors(nodeId);
//        set<int> neighborIds;
//        for (const Edge& edge : neighbors) {
//            neighborIds.insert(edge.destination);
//        }
//        TEST_CHECK(neighbors.size() == neighborIds.size());
//    }
}

TEST_LIST = {
    { "Vamana Algorithm", test_vamana },
    { NULL, NULL }
};
