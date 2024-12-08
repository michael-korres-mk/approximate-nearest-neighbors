#include "../../../include/acutest.h"
#include "../../main/cpp/utility/FilterGraph/FilterGraph.h"
#include "../../main/cpp/utility/DataPoint/DataPoint.h"
#include <vector>

void test_additionIsSuccess() {
    // Arrange
    vector<int> vec = {1, 2, 3};
    DataPoint<int> data = DataPoint(0,-1,-1,vec);

    FilterGraph<int> graph({}, 1, 5, 3, 1.2,-1);

    // Act
    graph.addVertex(data);

    // Assert
    DataPoint<int> vertex = graph.getVertex(0);
    TEST_ASSERT(vertex.vec.size() == data.vec.size());
    TEST_ASSERT(graph.equals(vertex.vec,data.vec) == 1 );

}

void test_invalidIdIsNullptr() {
    // Arrange
    vector<int> vec = {1, 2, 3};
    DataPoint<int> data = DataPoint(0,-1,-1,vec);
    FilterGraph<int> graph({}, 1, 5, 3, 1.2,-1);

    // Act
    graph.addVertex(data);

    // Assert
    DataPoint<int> vertex = graph.getVertex(113);
    TEST_ASSERT(vertex.vec.size() == 0);
}

TEST_LIST = {
    { "addVertex", test_additionIsSuccess },
    { "Add Vertex - Weight added properly", test_invalidIdIsNullptr },
    { NULL, NULL }
};