#include "../../../include/acutest.h"
#include "../../main/cpp/ann/Graph/Graph.h"
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
        TEST_ASSERT(result[i].destination == expectedNeighbors[i]);
    }

}

void test_robustPrune2() {
    vector<vector<int>> vec = {{0, 2}, {2, 5}, {2, 10}, {-1, 8}, {5, 6}, {7, 20}, {1, 2}};
    int R = 3;
    Graph<int> graph({}, 1, R, 4, 1.2);
    for (auto v : vec){
      graph.addVertex(v);
    }

    graph.addEdge(0, 5, graph.euclideanDistance(vec[0], vec[5]));
    graph.addEdge(0, 4, graph.euclideanDistance(vec[0], vec[4]));
    graph.addEdge(0, 2, graph.euclideanDistance(vec[0], vec[2]));

    graph.addEdge(1, 4, graph.euclideanDistance(vec[1], vec[4]));
    graph.addEdge(1, 6, graph.euclideanDistance(vec[1], vec[6]));
    graph.addEdge(1, 2, graph.euclideanDistance(vec[1], vec[2]));

    graph.addEdge(2, 1, graph.euclideanDistance(vec[2], vec[1]));
    graph.addEdge(2, 3, graph.euclideanDistance(vec[2], vec[3]));
    graph.addEdge(2, 4, graph.euclideanDistance(vec[2], vec[4]));

    graph.addEdge(3, 2, graph.euclideanDistance(vec[3], vec[2]));
    graph.addEdge(3, 0, graph.euclideanDistance(vec[3], vec[0]));
    graph.addEdge(3, 5, graph.euclideanDistance(vec[3], vec[5]));

    graph.addEdge(4, 0, graph.euclideanDistance(vec[4], vec[0]));
    graph.addEdge(4, 1, graph.euclideanDistance(vec[4], vec[1]));
    graph.addEdge(4, 6, graph.euclideanDistance(vec[4], vec[6]));

    graph.addEdge(5, 1, graph.euclideanDistance(vec[5], vec[1]));
    graph.addEdge(5, 6, graph.euclideanDistance(vec[5], vec[6]));
    graph.addEdge(5, 4, graph.euclideanDistance(vec[5], vec[4]));

    graph.addEdge(6, 2, graph.euclideanDistance(vec[6], vec[2]));
    graph.addEdge(6, 1, graph.euclideanDistance(vec[6], vec[1]));
    graph.addEdge(6, 3, graph.euclideanDistance(vec[6], vec[3]));

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