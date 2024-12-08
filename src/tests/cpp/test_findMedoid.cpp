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

void test_findMedoid() {
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

    // Act
    std::map<int, int> M = graph.findMedoid();

    // Assert
    // Φίλτρο 0: Medoid ID μπορεί να είναι 0, 3, 6, ή 9
    // Φίλτρο 1: Medoid ID πρέπει να είναι 1
    // Φίλτρο 2: Medoid ID πρέπει να είναι 2

    // Έλεγχος ότι υπάρχουν ακριβώς 3 φίλτρα στον χάρτη M
    TEST_CHECK(M.size() == 3);

    // Έλεγχος για Φίλτρο 0
    auto it0 = M.find(0);
    std::set<int> validMedoids0 = {0, 3, 6, 9};
    TEST_CHECK(validMedoids0.find(it0->second) != validMedoids0.end());

    // Έλεγχος για Φίλτρο 1
    auto it1 = M.find(1);
    TEST_CHECK(it1 != M.end());
    TEST_CHECK(it1->second == 1);

    // Έλεγχος για Φίλτρο 2
    auto it2 = M.find(2);
    TEST_CHECK(it2 != M.end());
    TEST_CHECK(it2->second == 2);
}

TEST_LIST = {
    { "FindMedoid", test_findMedoid },
    { NULL, NULL }
};
