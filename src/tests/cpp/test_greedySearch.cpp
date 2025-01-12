//
// Created by mkorres on 11/3/2024.
//

#include "../../../include/acutest.h"
#include "../../main/cpp/utility/FilterGraph/FilterGraph.h"
#include "../../main/cpp/utility/DataPoint/DataPoint.h"
#include <vector>
#include <iostream>

using namespace std;

void test_greedySearch() {
    // Arrange
    DataPoint<int> data0(0,-1,-1,vector<int>({0, 0}));
    DataPoint<int> data1(1,-1,-1,vector<int>({0, 1}));
    DataPoint<int> data2(2,-1,-1,vector<int>({0, 2}));
    DataPoint<int> data3(3,-1,-1,vector<int>({0, 3}));
    DataPoint<int> data4(4,-1,-1,vector<int>({0, 4}));
    DataPoint<int> data5(5,-1,-1,vector<int>({0, 5}));
    DataPoint<int> data6(6,-1,-1,vector<int>({0, 6}));
    DataPoint<int> data7(7,-1,-1,vector<int>({0, 7}));
    DataPoint<int> data8(8,-1,-1,vector<int>({0, 8}));
    DataPoint<int> data9(9,-1,-1,vector<int>({0, 9}));

    vector<DataPoint<int>> data = {data0,data1,data2,data3,data4,data5,data6,data7,data8,data9};

    FilterGraph<int> graph(data, 1, 5, 3,3,-1,0);

    for (unsigned int i = 0; i < data.size(); ++i) {
        for (unsigned int j = i + 1; j < data.size(); ++j) {
            graph.addEdge(i, j,graph.euclideanDistance(graph.getVertex(i).vec,graph.getVertex(j).vec)); // Adding edges between all vertices
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