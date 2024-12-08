//
// Created by mkorres on 12/7/2024.
//

#include "../../../include/acutest.h"
#include "../../main/cpp/ann/Graph/Graph.h"
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
# include "../../main/cpp/filterann/FilterDataset/FilterDataset.h"
# include "../../main/cpp/utility/Utils/Utils.h"
# include "../../main/cpp/utility/DataSet/DataSet.h"
#include "../../main/cpp/utility/VamanaContainer/VamanaContainer.h"
#include "../../main/cpp/filterann/FilterGraph/FilterGraph.h"
#include "../../main/cpp/utility/DataPoint/DataPoint.h"

void test_filterannimport() {

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

    string filename = "imextest_graph.bin";

    // Act
    graph.exportFilterGraph(filename);

    FilterGraph<int> importedGraph({}, 1, 5, 3, 3, 3);
    importedGraph.importFilterGraph(filename);

    // Assert
    TEST_ASSERT(graph.numOfDatapoints == importedGraph.numOfDatapoints);
    TEST_ASSERT(graph.L == importedGraph.L);
    TEST_ASSERT(graph.R == importedGraph.R);
    TEST_ASSERT(graph.k == importedGraph.k);
    TEST_ASSERT(graph.a == importedGraph.a);

    // Compare vertexMap
    TEST_ASSERT(graph.vertexMap.size() == importedGraph.vertexMap.size());
    for (const auto& [id, dp] : graph.vertexMap) {
        const auto& importedDp = importedGraph.vertexMap.at(id);
        TEST_ASSERT(dp.id == importedDp.id);
        TEST_ASSERT(dp.C == importedDp.C);
        TEST_ASSERT(dp.T == importedDp.T);

        for(unsigned int j = 0; j < importedDp.vec.size(); j++) {
            TEST_ASSERT(dp.vec[j] == importedDp.vec[j]);
        }
    }

    // Compare adjacency list (graph edges)
    TEST_ASSERT(graph.g.size() == importedGraph.g.size());
    for (const auto& [id, edges] : graph.g) {
        const auto& importedEdges = importedGraph.g.at(id);
        TEST_ASSERT(edges.size() == importedEdges.size());
        for (size_t i = 0; i < edges.size(); ++i) {
            TEST_ASSERT(edges[i].destination == importedEdges[i].destination);
            TEST_ASSERT(edges[i].weight == importedEdges[i].weight);
        }
    }

}

TEST_LIST = {
    { "test filterann import", test_filterannimport },
    { NULL, NULL }
};