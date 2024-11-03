//
// Created by mkorres on 11/3/2024.
//

#include "../include/acutest.h"
#include "../src/main/cpp/utility/Graph/Graph.h"
#include <vector>

using namespace std;

void test_greedySearch() {
    // Arrange
    vector<int> data0 = {1, 2, 3};
    vector<int> data1 = {4, 5, 6};
    vector<int> data2 = {7, 8, 9};
    vector<int> data3 = {10, 11, 12};

    Graph<int> graph(vector<vector<int>>{data0,data1,data2,data3},150,5, 3, 1.2);

    graph.addEdge(0, 1, graph.euclideanDistance(data0, data1));
    graph.addEdge(0, 2, graph.euclideanDistance(data0, data2));
    graph.addEdge(0, 3, graph.euclideanDistance(data0, data3));
    graph.addEdge(1, 2, graph.euclideanDistance(data1, data2));



    TEST_ASSERT(false);
}

void test_euclideanDistance() {
    // Arrange
    vector<int> v1 = {1, 2};
    vector<int> v2 = {4, 6};

    // Act
    double distance = Graph<int>::euclideanDistance(v1, v2);

    // Assert
    TEST_ASSERT(fabs(distance - 5.0) < 1e-6); // Αναμένουμε απόσταση 5.0
}

// Λίστα με όλα τα tests
TEST_LIST = {
    // { "addVertex", test_addVertex },
    // { "addEdge", test_addEdge },
    // { "getOutNeighbors", test_getOutNeighbors },
    // { "initializeRandomGraph", test_initializeRandomGraph },
    // { "findMedoid", test_findMedoid },
    // { "findMedoidSample", test_findMedoidSample },
    // { "vamanaIndexing", test_vamanaIndexing },
    // { "robustPrune", test_robustPrune },
    // { "greedySearch", test_greedySearch },
    // { "setOutNeighbors", test_setOutNeighbors },
    // { "addOutNeighbor", test_addOutNeighbor },
    { "euclideanDistance", test_euclideanDistance },
    { NULL, NULL } // Τερματισμός της λίστας
};
