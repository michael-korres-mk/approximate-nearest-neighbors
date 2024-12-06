//
// Created by mkorres on 12/6/2024.
//

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
# include "../filterann/FilterQuerySet/FilterQuerySet.h"
# include "../filterann/FilterDataset/FilterDataset.h"
# include "../utility/Utils/Utils.h"
# include "../utility/DataSet/DataSet.h"
#include "../utility/VamanaContainer/VamanaContainer.h"
#include "../filterann/FilterGraph/FilterGraph.h"
#include "../filterann/DataPoint/DataPoint.h"

using namespace std;

int main() {
    // Step 1: Create a test graph with known data
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

    graph.exportFilterGraph(filename);

    FilterGraph<int> importedGraph({}, 1, 5, 3, 3, 3);
    importedGraph.importFilterGraph(filename);

    // Compare metadata
    assert(graph.numOfDatapoints == importedGraph.numOfDatapoints);
    assert(graph.L == importedGraph.L);
    assert(graph.R == importedGraph.R);
    assert(graph.k == importedGraph.k);
    assert(graph.a == importedGraph.a);


    int a = graph.vertexMap.size();
    int b = importedGraph.vertexMap.size();
    PRINT_VAR(a)
    PRINT_VAR(b)

    // Compare vertexMap
    assert(graph.vertexMap.size() == importedGraph.vertexMap.size());
    for (const auto& [id, dp] : graph.vertexMap) {
        const auto& importedDp = importedGraph.vertexMap.at(id);
        assert(dp.id == importedDp.id);
        assert(dp.C == importedDp.C);
        assert(dp.T == importedDp.T);

        for(unsigned int j = 0; j < importedDp.vec.size(); j++) {
            assert(dp.vec[j] == importedDp.vec[j]);
        }
    }

    // Compare adjacency list (graph edges)
    assert(graph.g.size() == importedGraph.g.size());
    for (const auto& [id, edges] : graph.g) {
        const auto& importedEdges = importedGraph.g.at(id);
        assert(edges.size() == importedEdges.size());
        for (size_t i = 0; i < edges.size(); ++i) {
            assert(edges[i].destination == importedEdges[i].destination);
            assert(edges[i].weight == importedEdges[i].weight);
        }
    }

    cout << "Test passed: The export and import methods are symmetric." << endl;

    return 0;
}
