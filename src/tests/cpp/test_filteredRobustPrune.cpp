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

void test_filteredRobustPrune() {
    // Arrange
    std::vector<int> data0 = {0, 0};
    std::vector<int> data1 = {0, 1};
    std::vector<int> data2 = {0, 2};
    std::vector<int> data3 = {0, 3};
    std::vector<int> data4 = {0, 4};
    std::vector<int> data5 = {0, 5};
    std::vector<int> data6 = {0, 6};
    std::vector<int> data7 = {0, 7};
    std::vector<int> data8 = {0, 8};
    std::vector<int> data9 = {0, 9};

    std::vector<DataPoint<int>> data = {
        DataPoint<int>(0,0, -1, data0), // ID 0
        DataPoint<int>(1,1, -1, data1), // ID 1
        DataPoint<int>(2,2, -1, data2), // ID 2
        DataPoint<int>(3,0, -1, data3), // ID 3
        DataPoint<int>(4,1, -1, data4), // ID 4
        DataPoint<int>(5,2, -1, data5), // ID 5
        DataPoint<int>(6,0, -1, data6), // ID 6
        DataPoint<int>(7,1, -1, data7), // ID 7
        DataPoint<int>(8,2, -1, data8), // ID 8
        DataPoint<int>(9,0, -1, data9)  // ID 9
    };

    FilterGraph<int> graph(data, 1, 5, 3, 3, 3);

    // Ενημερώνουμε τον γράφο με κάποιες ακμές
    graph.addEdge(0, 1, 1.0);
    graph.addEdge(0, 2, 2.0);
    graph.addEdge(0, 3, 3.0);

    std::vector<int> candidateNeighbors = {1, 2, 3, 4, 5};

    // Act
    std::vector<Edge> result = graph.filteredRobustPrune(0, candidateNeighbors, 2.5, 2);

    // Assert
    TEST_CHECK(result.size() == 2); // Ελέγχει ότι το μέγεθος είναι σωστό.
    TEST_CHECK((result[0].destination == 1 || result[1].destination == 1));
    TEST_CHECK((result[0].destination == 2 || result[1].destination == 2));



    // Arrange2
    FilterGraph<int> graph2(data, 1, 5, 3, 3.0, 3);

    // Act2
    int p = 0;
    std::vector<int> V = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    double a = 3.0;
    int R = 3;

    std::vector<Edge> result2 = graph2.filteredRobustPrune(p, V, a, R);

    // Assert2
    TEST_CHECK(result2.size() == 3);
    TEST_CHECK(result2[0].destination == 1);
    TEST_CHECK(result2[1].destination == 2);
    TEST_CHECK(result2[2].destination == 3);

    // Έλεγχος αποστάσεις
    TEST_CHECK(fabs(result2[0].weight - 1.0) < 1e-6);
    TEST_CHECK(fabs(result2[1].weight - 2.0) < 1e-6);
    TEST_CHECK(fabs(result2[2].weight - 3.0) < 1e-6);
}

TEST_LIST = {
    { "Filtered Robust Prune", test_filteredRobustPrune },
    { NULL, NULL }
};
