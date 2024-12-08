#include "../../../include/acutest.h"
#include "../../main/cpp/utility/VamanaContainer/VamanaContainer.h"
#include <vector>
#include <iostream>

void test_insert(void) {
    VamanaContainer container(3);

    container.insert({1, 0.5});
    container.insert({2, 0.3});
    container.insert({3, 0.7});

    TEST_CHECK(container.subset(-1).size() == 3);

    container.insert({4, 0.9});
    TEST_CHECK(container.subset(-1).size() == 3);
    TEST_CHECK(!container.contains(4));

    container.insert({5, 0.2});
    TEST_CHECK(container.contains(5));
    TEST_CHECK(!container.contains(3));
}

void test_contains(void) {
    VamanaContainer container(3);

    container.insert({1, 0.5});
    container.insert({2, 0.3});

    TEST_CHECK(container.contains(1));
    TEST_CHECK(container.contains(2));
    TEST_CHECK(!container.contains(3));
}

void test_subset(void) {
    VamanaContainer container(5);

    container.insert({1, 0.5});
    container.insert({2, 0.3});
    container.insert({3, 0.7});
    container.insert({4, 0.2});
    container.insert({5, 0.6});

    auto subset1 = container.subset(3);
    TEST_CHECK(subset1.size() == 3);
    TEST_CHECK(subset1[0] == 4);
    TEST_CHECK(subset1[1] == 2);
    TEST_CHECK(subset1[2] == 1);

    auto subset2 = container.subset(-1);
    TEST_CHECK(subset2.size() == 5);
}

void test_print(void) {
    VamanaContainer container(3);

    container.insert({1, 0.5});
    container.insert({2, 0.3});
    container.insert({3, 0.7});

    TEST_MSG("Contents of the container:");
    container.print();
}

TEST_LIST = {
    {"Test Insert", test_insert},
    {"Test Contains", test_contains},
    {"Test Subset", test_subset},
    {"Test Print", test_print},
    { NULL, NULL }
};
