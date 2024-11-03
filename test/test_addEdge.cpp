#include "acutest.h"
#include "Graph/Graph.h"
#include <vector>

void test_addEdge() {
    Graph<int> graph({}, 5, 3, 0.5);
    vector<int> data1 = {9, 2, 2};
    vector<int> data2 = {6, 6, 2};

    graph.addVertex(data1); // ID = 0
    graph.addVertex(data2); // ID = 1

    graph.addEdge(0, 1, graph.euclideanDistance(data1, data2));
    vector<Edge> neighbors = graph.getNeighbors(0);      // Ο κόμβος 0 έχει τον κόμβο 1 ως γείτονα
    TEST_ASSERT(neighbors.size() == 1);
    TEST_ASSERT(neighbors[0].getDestination() == 1);

    neighbors = graph.getNeighbors(1);        // Ο κόμβος 1 δεν έχει γείτονες
    TEST_ASSERT(neighbors.size() == 0);

    const vector<Edge>& edges = graph.getEdges(0);    // Έλεγχος του βάρους της ακμής
    TEST_ASSERT(edges.size() == 1);
    TEST_ASSERT(edges[0].getDestination() == 1);
    TEST_ASSERT(edges[0].getWeight() == 5.0);

    const vector<Edge>& edges1 = graph.getEdges(1);    // Ο κόμβος 1 δεν υπάρχουν απροσδόκητες ακμές
    TEST_ASSERT(edges1.size() == 0);

    // Προσθήκη ακμής από τον κόμβο 1 στον κόμβο 0
    graph.addEdge(1, 0, graph.euclideanDistance(data2, data1));


    neighbors = graph.getNeighbors(1);       // Ο κόμβος 1 έχει πλέον έναν γείτονα
    TEST_ASSERT(neighbors.size() == 1);
    TEST_ASSERT(neighbors[0].getDestination() == 0);

    const vector<Edge>& edges1_updated = graph.getEdges(1);     // Έλεγχος του βάρους της νέας ακμής
    TEST_ASSERT(edges1_updated.size() == 1);
    TEST_ASSERT(edges1_updated[0].getDestination() == 0);
    TEST_ASSERT(edges1_updated[0].getWeight() == 5.0);


    const vector<Edge>& edges0_updated = graph.getEdges(0);        // Ο κόμβος 0 εξακολουθεί να έχει μία ακμή
    TEST_ASSERT(edges0_updated.size() == 1);


//    // Προσπάθεια προσθήκης ακμής σε μη υπαρκτό κόμβο
//    try {
//        graph.addEdge(0, 100, 1.0f);
//        TEST_ASSERT(false);
//    } catch (const std::out_of_range& e) {
//        TEST_ASSERT(true);
//    } catch (...) {
//        TEST_ASSERT(false);
//    }

    TEST_ASSERT(graph.getTotalEdges() == 2);    // Έλεγχος της ακεραιότητας του γράφου

//    graph.addEdge(0, 1, graph.euclideanDistance(data1, data2));        // Έλεγχος για διπλές ακμές
//    TEST_ASSERT(graph.getTotalEdges() == 2);
}

TEST_LIST = {
    { "addEdge", test_addEdge },
    { NULL, NULL }
};
