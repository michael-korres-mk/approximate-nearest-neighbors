#include "../include/acutest.h"
#include "../src/main/cpp/utility/Graph/Graph.h"
#include <vector>

void test_additionIsSuccess() {
    // Arrange
    vector<int> data1 = {1, 2, 3};
    vector<int> data2 = {4, 5, 6};
    Graph<int> graph({data1,data2}, 1, 5, 3, 1.2);

    // Act
    graph.addEdge(0, 1, graph.euclideanDistance(data1, data2));
    vector<Edge> neighborsOf0 = graph.getNeighbors(0);      // Ο κόμβος 0 έχει τον κόμβο 1 ως γείτονα
    vector<Edge> neighborsOf1 = graph.getNeighbors(1);

    // Assert
    TEST_ASSERT(neighborsOf0.size() == 1);
    TEST_ASSERT(neighborsOf0[0].destination == 1);
    TEST_ASSERT(neighborsOf1.size() == 0);

}

void test_weightAddedProperly() {
    // Arrange
    vector<int> data1 = {1, 2, 3};
    vector<int> data2 = {4, 5, 6};
    Graph<int> graph({data1,data2}, 1, 5, 3, 1.2);

    // Act
    graph.addEdge(0, 1, graph.euclideanDistance(data1, data2));
    vector<Edge> neighborsOf0 = graph.getNeighbors(0);      // Ο κόμβος 0 έχει τον κόμβο 1 ως γείτονα

    // Assert
    Edge addedEdge = neighborsOf0[0];
    double edgeWeight = addedEdge.weight;    // Έλεγχος του βάρους της ακμής
    double epsilon = 1e-6;
    TEST_ASSERT(fabs((edgeWeight) - (0.5)) > (epsilon));
}

TEST_LIST = {
    { "Add Edge - Addition is success", test_additionIsSuccess },
    { "Add Edge - Weight added properly", test_weightAddedProperly },
    { NULL, NULL }
};
