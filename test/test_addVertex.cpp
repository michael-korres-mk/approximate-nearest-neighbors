#include "acutest.h"
#include "Graph/Graph.h"
#include <vector>

void test_addVertex() {
    Graph<int> graph({}, 5, 3, 0.5);
    vector<int> data1 = {1, 2, 3};
    vector<int> data2 = {4, 5, 6};

    TEST_ASSERT(graph.getTotalVertices() == 0);

    graph.addVertex(data1);        // ID = 0
    TEST_ASSERT(graph.getTotalVertices() == 1);

    graph.addVertex(data2);        // ID = 1
    TEST_ASSERT(graph.getTotalVertices() == 2);

    TEST_ASSERT(graph.getNeighbors(0).size() == 0);
    TEST_ASSERT(graph.getNeighbors(1).size() == 0);

    const vector<int>& retrievedData1 = graph.getVertexData(0);
    TEST_ASSERT(retrievedData1 == data1);

    const vector<int>& retrievedData2 = graph.getVertexData(1);
    TEST_ASSERT(retrievedData2 == data2);

    graph.addVertex(data2);        // ID = 2
    TEST_ASSERT(graph.getTotalVertices() == 3 );

    const vector<int>& retrievedData3 = graph.getVertexData(2);
    TEST_ASSERT(retrievedData3 == data2);

    for (int i = 0; i < 222; i++)
        graph.addVertex({i, i+1, i+2});
    TEST_ASSERT(graph.getTotalVertices() == 225 );

    const vector<int>& retrievedData200 = graph.getVertexData(200);
    vector<int> data200 = {197, 198, 199};
    TEST_ASSERT(retrievedData200 == data200);
}

TEST_LIST = {
    { "addVertex", test_addVertex },
    { NULL, NULL }
};
