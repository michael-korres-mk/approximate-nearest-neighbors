#include "acutest.h"
#include "Graph/Graph.h"
#include <vector>
#include <random>

void test_initializeRandomEdge() {
    int R = 20;
    Graph<int> graph({}, R, 3, 0.5);
    int num_points = 100; // Αριθμός σημείων (μεγαλύτερο από 1000)
    mt19937 rng(42); // Σταθερός seed για αναπαραγωγή
    uniform_int_distribution<int> dist(-100, 100);

    // arrange
    // Προσθήκη τυχαίων σημείων στον γράφο
    for (int i = 0; i < num_points; ++i) {
        vector<int> point = {dist(rng), dist(rng)};
        graph.addVertex(point);
    }

    // Αρχικοποίηση τυχαίου γραφήματος
    graph.initializeRandomEdges();               //   <-------Προβλημα

    // Έλεγχος για το αν τηρείται το όριο γειτόνων R
    const auto& vertexMap = graph.getVertexMap();
    for (const auto& [vertex, neighbors] : vertexMap) {
        vector<Edge> outNeighbors = graph.getNeighbors(vertex);
        TEST_ASSERT(outNeighbors.size() <= R);
    }

    // Έλεγχος για self-loops
    for (const auto& [vertex, neighbors] : vertexMap) {
        vector<Edge> outNeighbors = graph.getNeighbors(vertex);
        for (Edge neighbor : outNeighbors) {
            TEST_ASSERT(neighbor.getDestination() != vertex);
        }
    }

    // Έλεγχος της τυχαιότητας με επανάληψη
    map<int, vector<Edge>> neighbors_snapshot;
    for (const auto& [vertex, neighbors] : vertexMap) {
        neighbors_snapshot[vertex] = graph.getNeighbors(vertex);
    }

    // Επανεκτέλεση της `initializeRandomGraph` και έλεγχος ότι οι γείτονες αλλάζουν
    graph.initializeRandomEdges();
    bool neighbors_changed = false;
    for (const auto& [vertex, old_neighbors] : neighbors_snapshot) {
        const auto& new_neighbors = graph.getNeighbors(vertex);

        // Σύγκριση στοιχείο-προς-στοιχείο
        if (new_neighbors.size() != old_neighbors.size()) {
            neighbors_changed = true;
            break;
        }

        for (size_t i = 0; i < new_neighbors.size(); ++i) {
            if (new_neighbors[i].getDestination() != old_neighbors[i].getDestination() ||
                new_neighbors[i].getWeight() != old_neighbors[i].getWeight()) {
                neighbors_changed = true;
                break;
                }
        }

        if (neighbors_changed) {
            break;
        }
    }
    TEST_ASSERT(neighbors_changed);
}

TEST_LIST = {
    { "initializeRandomEdge", test_initializeRandomEdge },
    { NULL, NULL }
};
