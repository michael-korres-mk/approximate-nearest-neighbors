#include "../include/acutest.h"
#include "../src/main/cpp/utility/Graph/Graph.h"
#include <vector>

void test_additionIsSuccess() {
    // Arrange
    vector<int> data = {1, 2, 3};
    Graph<int> graph({}, 1, 5, 3, 1.2);

    // Act
    graph.addVertex(data);

    // Assert
    vector<int> vertex = graph.getVertex(0);
    TEST_ASSERT(vertex.size() == data.size());
    TEST_ASSERT(graph.equals(vertex,data) == 1 );

}

void test_invalidIdIsNullptr() {
    // Arrange
    vector<int> data = {1, 2, 3};
    Graph<int> graph({}, 1, 5, 3, 1.2);

    // Act
    graph.addVertex(data);

    // Assert
    vector<int> vertex = graph.getVertex(113);
    TEST_ASSERT(vertex.size() == 0);
}

TEST_LIST = {
    { "addVertex", test_additionIsSuccess },
//    { "Add Vertex - Weight added properly", test_invalidIdIsNullptr },
    { NULL, NULL }
};