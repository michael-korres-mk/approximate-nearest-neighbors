//
// Created by mkorres on 11/3/2024.
//

#include "../../../include/acutest.h"
#include "../../main/cpp/ann/Graph/Graph.h"
#include <vector>
#include <iostream>

using namespace std;

void test_greedySearch() {
    // Arrange
    vector<int> data0 = {0, 0};
    vector<int> data1 = {0, 1};
    vector<int> data2 = {0, 2};
    vector<int> data3 = {0, 3};
    vector<int> data4 = {0, 4};
    vector<int> data5 = {0, 5};
    vector<int> data6 = {0, 6};
    vector<int> data7 = {0, 7};
    vector<int> data8 = {0, 8};
    vector<int> data9 = {0, 9};

    vector<vector<int>> data = {data0,data1,data2,data3,data4,data5,data6,data7,data8,data9};

    Graph<int> graph(data, 1, 5, 3,3);

    for (int i = 0; i < data.size(); ++i) {
        for (int j = i + 1; j < data.size(); ++j) {
            graph.addEdge(i, j,Graph<int>::euclideanDistance(graph.getVertex(i),graph.getVertex(j))); // Adding edges between all vertices
        }
    }
    int startVertex = 0;
    vector<int> query = {0, 1};
    int k = 3;
    int L = 5;

    vector<int> expectedVisitedVec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> expectedSubset = {0, 1, 4};

    // Act
    pair<vector<int>,vector<int>> result = graph.greedySearch(startVertex, query, k, L);
    auto visited = result.first;
    auto visitedVec = result.second;

    for (size_t i = 0; i < visitedVec.size(); ++i) {
        TEST_ASSERT(visitedVec[i] == expectedVisitedVec[i]);
    }

    // Check if the subset found is correct
    for (size_t i = 0; i < visited.size(); ++i) {
        TEST_ASSERT(visited[i] == expectedSubset[i]);
    }

}

TEST_LIST = {
    { "Greedy Search", test_greedySearch },
    { NULL, NULL }
};