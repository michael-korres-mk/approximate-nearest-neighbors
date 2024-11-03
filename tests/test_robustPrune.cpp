#include "../include/acutest.h"
#include "../src/main/cpp/utility/Graph/Graph.h"
#include <vector>
#include <iostream>

using namespace std;

void test_robustPrune() {
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

    int p = 0;
    std::vector<int> V = graph.getVerticesIds();
    double a = 3.0;
    int R = 10;

    std::vector<int> expectedNeighbors = {1, 2, 4, 7};

    // Act
    vector<Edge> result = graph.robustPrune(p,V,a,R);

    for (int i = 0; i < result.size(); i++) {
        TEST_ASSERT(result[i].getDestination() == expectedNeighbors[i]);
    }

}

TEST_LIST = {
    { "RobustPrune", test_robustPrune },
    { NULL, NULL }
};