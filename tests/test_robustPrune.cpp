#include "../include/acutest.h"
#include "../src/main/cpp/utility/Graph/Graph.h"
#include <vector>

void test_robustPrune() {
    Graph<int> graph({}, 1, 5, 3, 1.2);
    vector<int> data0 = {0, 2};
    vector<int> data1 = {2, 5};
    vector<int> data2 = {2, 10};
    vector<int> data3 = {-1, 8};
    vector<int> data4 = {5, 6};
    vector<int> data5 = {7, 20};
    vector<int> data6 = {1, 2};
    vector<int> data7 = {-7,2};
    vector<int> data8 = {1, 8};
    vector<int> data9 = {5, 2};
    graph.addVertex(data0);        // ID = 0
    graph.addVertex(data1);        // ID = 1
    graph.addVertex(data2);        // ID = 2
    graph.addVertex(data3);        // ID = 3
    graph.addVertex(data4);        // ID = 4
    graph.addVertex(data5);        // ID = 5
    graph.addVertex(data6);        // ID = 6
    graph.addVertex(data7);        // ID = 7
    graph.addVertex(data8);        // ID = 8
    graph.addVertex(data9);        // ID = 9


    graph.initializeRandomEdges();

    const auto& vertexMap = graph.getVertexMap();
    for (const auto& [vertex, neighbors] : vertexMap) {
        vector<Edge> outNeighbors = graph.getNeighbors(vertex);
        for (const auto& neighbor : outNeighbors) {
          cout << vertex << ", " << neighbor.getDestination() << endl;
        }
    }


    TEST_ASSERT(false); // Εδώ θα προσθέσεις τον κανονικό κώδικα του test
}

TEST_LIST = {
    { "RobustPrune", test_robustPrune },
    { NULL, NULL }
};