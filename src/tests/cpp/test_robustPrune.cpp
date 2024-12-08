#include "../../../include/acutest.h"
#include "../../main/cpp/utility/FilterGraph/FilterGraph.h"
#include "../../main/cpp/utility/DataPoint/DataPoint.h"
#include <vector>
#include <iostream>

using namespace std;

void test_robustPrune() {
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

    FilterGraph<int> graph(data, 1, 5, 3,3,-1);

    int p = 0;
    std::vector<int> V = graph.getVerticesIds();
    double a = 3.0;
    int R = 10;

    std::vector<int> expectedNeighbors = {1, 2, 4, 7};

    // Act
    vector<Edge> result = graph.robustPrune(p,V,a,R);

    for (unsigned int i = 0; i < result.size(); i++) {
        TEST_ASSERT(result[i].destination == expectedNeighbors[i]);
    }

}

void test_robustPrune2() {
    DataPoint<int> data0(0,-1,-1,vector<int>({0, 2}));
    DataPoint<int> data1(1,-1,-1,vector<int>({2, 5}));
    DataPoint<int> data2(2,-1,-1,vector<int>({2, 10}));
    DataPoint<int> data3(3,-1,-1,vector<int>({-1, 8}));
    DataPoint<int> data4(4,-1,-1,vector<int>({5, 6}));
    DataPoint<int> data5(5,-1,-1,vector<int>({7, 20}));
    DataPoint<int> data6(6,-1,-1,vector<int>({1, 2}));

    vector<DataPoint<int>> data = {data0,data1,data2,data3,data4,data5,data6};

    int R = 3;
    FilterGraph<int> graph({}, 1, R, 4, 1.2,-1);
    for (auto d : data) graph.addVertex(d);

    graph.addEdge(0, 5, graph.euclideanDistance(data[0].vec, data[5].vec));
    graph.addEdge(0, 4, graph.euclideanDistance(data[0].vec, data[4].vec));
    graph.addEdge(0, 2, graph.euclideanDistance(data[0].vec, data[2].vec));

    graph.addEdge(1, 4, graph.euclideanDistance(data[1].vec, data[4].vec));
    graph.addEdge(1, 6, graph.euclideanDistance(data[1].vec, data[6].vec));
    graph.addEdge(1, 2, graph.euclideanDistance(data[1].vec, data[2].vec));

    graph.addEdge(2, 1, graph.euclideanDistance(data[2].vec, data[1].vec));
    graph.addEdge(2, 3, graph.euclideanDistance(data[2].vec, data[3].vec));
    graph.addEdge(2, 4, graph.euclideanDistance(data[2].vec, data[4].vec));

    graph.addEdge(3, 2, graph.euclideanDistance(data[3].vec, data[2].vec));
    graph.addEdge(3, 0, graph.euclideanDistance(data[3].vec, data[0].vec));
    graph.addEdge(3, 5, graph.euclideanDistance(data[3].vec, data[5].vec));

    graph.addEdge(4, 0, graph.euclideanDistance(data[4].vec, data[0].vec));
    graph.addEdge(4, 1, graph.euclideanDistance(data[4].vec, data[1].vec));
    graph.addEdge(4, 6, graph.euclideanDistance(data[4].vec, data[6].vec));

    graph.addEdge(5, 1, graph.euclideanDistance(data[5].vec, data[1].vec));
    graph.addEdge(5, 6, graph.euclideanDistance(data[5].vec, data[6].vec));
    graph.addEdge(5, 4, graph.euclideanDistance(data[5].vec, data[4].vec));

    graph.addEdge(6, 2, graph.euclideanDistance(data[6].vec, data[2].vec));
    graph.addEdge(6, 1, graph.euclideanDistance(data[6].vec, data[1].vec));
    graph.addEdge(6, 3, graph.euclideanDistance(data[6].vec, data[3].vec));

    int p = 1;
    vector<int> V = {2, 3, 5, 6};
    double a = 1.3;
    vector<Edge> rp = graph.robustPrune(p, V, a, R);
    // 1, 2 -> 5
    // 1, 3 -> 4.24264
    // 1, 4 -> 3.16228
    // 1, 5 -> 15.8114
    // 1, 6 -> 3.16228
    TEST_ASSERT(rp[0].destination == 6);
    TEST_ASSERT(rp[1].destination == 4);
    TEST_ASSERT(rp[2].destination == 3);


    int p2 = 6;
    vector<int> V2 = {1, 3, 5, 6};
    vector<Edge> rp2 = graph.robustPrune(p2, V2, a, R);
    // 6, 1 -> 3.16228          |  p_star  |
    // 6, 2 -> 8.06226          |  p' = 2  |  1,3*d(p_star, p') <= d(p2, p')  <=>  1.3*d(1, 2) <= d(6, 2)  <=>  1.3*5       <= 8.06226   |  remove
    // 6, 3 -> 6.32456          |  p' = 3  |  1,3*d(p_star, p') <= d(p2, p')  <=>  1.3*d(1, 3) <= d(6, 3)  <=>  1.3*4.24264 <= 6.32456   |  remove
    // 6, 5 -> 18.9737          |  p' = 5  |  1,3*d(p_star, p') <= d(p2, p')  <=>  1.3*d(1, 5) <= d(6, 5)  <=>  1.3*15.8114 <= 18.9737   |  not  remove
    //
    //                  5       |  p_star  |
    TEST_ASSERT(rp2[0].destination == 1);
    TEST_ASSERT(rp2[1].destination == 5);

    int p3 = 6;
    vector<int> V3 = {1, 3, 5, 6};
    a = 2;
    vector<Edge> rp3 = graph.robustPrune(p3, V3, a, R);
    // 6, 1 -> 3.16228          |  p_star  |
    // 6, 2 -> 8.06226          |  p' = 2  |  2*d(p_star, p') <= d(p2, p')  <=>  2*d(1, 2) <= d(6, 2)  <=>  2*5       <= 8.06226   |  not  remove
    // 6, 3 -> 6.32456          |  p' = 3  |  2*d(p_star, p') <= d(p2, p')  <=>  2*d(1, 3) <= d(6, 3)  <=>  2*4.24264 <= 6.32456   |  not  remove
    // 6, 5 -> 18.9737          |  p' = 5  |  2*d(p_star, p') <= d(p2, p')  <=>  2*d(1, 5) <= d(6, 5)  <=>  2*15.8114 <= 18.9737   |  not  remove
    //
    //                  2       |  p' = 2  |  2*d(p_star, p') <= d(p2, p')  <=>  2*d(3, 2) <= d(6, 2)  <=>  2*3.60555 <= 8.06226   |  remove
    //                  3       |  p_star  |
    //                  5       |  p' = 5  |  2*d(p_star, p') <= d(p2, p')  <=>  2*d(3, 5) <= d(6, 5)  <=>  2*14.4222 <= 18.9737   |  not  remove
    //
    //                  5       |  p_star  |
    TEST_ASSERT(rp3[0].destination == 1);
    TEST_ASSERT(rp3[1].destination == 3);
    TEST_ASSERT(rp3[2].destination == 5);

}


TEST_LIST = {
    { "RobustPrune", test_robustPrune },
    { "RobustPrune2", test_robustPrune2 },
    { NULL, NULL }
};