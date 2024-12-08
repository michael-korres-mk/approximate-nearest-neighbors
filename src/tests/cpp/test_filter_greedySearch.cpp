//
// Created by mkorres on 11/3/2024.
//

#include "../../../include/acutest.h"
#include "../../main/cpp/utility/FilterGraph/FilterGraph.h"
#include <vector>
#include <iostream>
#include <map>
#include <vector>
#include <cassert>
# include <chrono>
# include <iostream>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>
# include "../../main/cpp/filterann/FilterQuerySet/FilterQuerySet.h"
# include "../../main/cpp/utility/FilterDataset/FilterDataset.h"
# include "../../main/cpp/utility/Utils/Utils.h"
# include "../../main/cpp/utility/DataSet/DataSet.h"
#include "../../main/cpp/utility/VamanaContainer/VamanaContainer.h"
# include "../../main/cpp/utility/FilterDataset/FilterDataset.h"
#include "../../main/cpp/utility/DataPoint/DataPoint.h"

using namespace std;

void test_filter_greedySearch() {
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

    vector<DataPoint<int>> data = {
        DataPoint<int>(0,0,-1,data0),
        DataPoint<int>(1,1,-1,data1),
        DataPoint<int>(2,2,-1,data2),
        DataPoint<int>(3,0,-1,data3),
        DataPoint<int>(4,1,-1,data4),
        DataPoint<int>(5,2,-1,data5),
        DataPoint<int>(6,0,-1,data6),
        DataPoint<int>(7,1,-1,data7),
        DataPoint<int>(8,2,-1,data8),
        DataPoint<int>(9,0,-1,data9)
    };

    FilterGraph<int> graph(data, 1, 5, 3, 3, 3);

    for (unsigned int i = 0; i < data.size(); ++i) {
        for (unsigned int j = i + 1; j < data.size(); ++j) {
            graph.addEdge(i, j,FilterGraph<int>::euclideanDistance(graph.getVertex(i).vec,graph.getVertex(j).vec)); // Adding edges between all vertices
        }
    }
    int startVertex = 0;
    vector<int> query = {0, 1};
    int k = 3;
    int L = 5;

    vector<int> expectedVisitedVec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> expectedSubset = {0, 3, 6, 9};



    // Act
    pair<vector<int>,vector<int>> result = graph.filteredGreedySearch({}, query, k, L,2);
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
    { "Filter Greedy Search", test_filter_greedySearch },
    { NULL, NULL }
};