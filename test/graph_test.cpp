#include "acutest.h"
#include "Graph/Graph.h"
#include <vector>

using namespace std;

void test_addVertex() {
    Graph<int> graph({}, 5, 3, 0.5, 10);
    vector<int> data1 = {1, 2, 3};
    vector<int> data2 = {4, 5, 6};

    TEST_ASSERT(graph.getTotalVertices() == 0);

    graph.addVertex(data1);        // ID = 0
    TEST_ASSERT(graph.getTotalVertices() == 1);

    graph.addVertex(data2);        // ID = 1
    TEST_ASSERT(graph.getTotalVertices() == 2);

    TEST_ASSERT(graph.getOutNeighbors(0).size() == 0);
    TEST_ASSERT(graph.getOutNeighbors(1).size() == 0);

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


void test_addEdge() {
    Graph<int> graph({}, 5, 3, 0.5, 10);
    vector<int> data1 = {9, 2, 2};
    vector<int> data2 = {6, 6, 2};

    graph.addVertex(data1); // ID = 0
    graph.addVertex(data2); // ID = 1

    graph.addEdge(0, 1, graph.euclideanDistance(data1, data2));
    vector<int> neighbors = graph.getOutNeighbors(0);      // Ο κόμβος 0 έχει τον κόμβο 1 ως γείτονα
    TEST_ASSERT(neighbors.size() == 1);
    TEST_ASSERT(neighbors[0] == 1);

    neighbors = graph.getOutNeighbors(1);        // Ο κόμβος 1 δεν έχει γείτονες
    TEST_ASSERT(neighbors.size() == 0);

    const vector<Edge>& edges = graph.getEdges(0);    // Έλεγχος του βάρους της ακμής
    TEST_ASSERT(edges.size() == 1);
    TEST_ASSERT(edges[0].getDestination() == 1);
    TEST_ASSERT(edges[0].getWeight() == 5.0);

    const vector<Edge>& edges1 = graph.getEdges(1);    // Ο κόμβος 1 δεν υπάρχουν απροσδόκητες ακμές
    TEST_ASSERT(edges1.size() == 0);

    // Προσθήκη ακμής από τον κόμβο 1 στον κόμβο 0
    graph.addEdge(1, 0, graph.euclideanDistance(data2, data1));


    neighbors = graph.getOutNeighbors(1);       // Ο κόμβος 1 έχει πλέον έναν γείτονα
    TEST_ASSERT(neighbors.size() == 1);
    TEST_ASSERT(neighbors[0] == 0);

    const vector<Edge>& edges1_updated = graph.getEdges(1);     // Έλεγχος του βάρους της νέας ακμής
    TEST_ASSERT(edges1_updated.size() == 1);
    TEST_ASSERT(edges1_updated[0].getDestination() == 0);
    TEST_ASSERT(edges1_updated[0].getWeight() == 5.0);


    const vector<Edge>& edges0_updated = graph.getEdges(0);        // Ο κόμβος 0 εξακολουθεί να έχει μία ακμή
    TEST_ASSERT(edges0_updated.size() == 1);


    // Προσπάθεια προσθήκης ακμής σε μη υπαρκτό κόμβο
    try {
        graph.addEdge(0, 100, 1.0f);
        TEST_ASSERT(false);
    } catch (const std::out_of_range& e) {
        TEST_ASSERT(true);
    } catch (...) {
        TEST_ASSERT(false);
    }

    TEST_ASSERT(graph.getTotalEdges() == 2);    // Έλεγχος της ακεραιότητας του γράφου

    graph.addEdge(0, 1, graph.euclideanDistance(data1, data2));        // Έλεγχος για διπλές ακμές
    TEST_ASSERT(graph.getTotalEdges() == 2);
}


void test_getOutNeighbors() {
    Graph<int> graph({}, 5, 3, 0.5, 10);
    vector<int> data0 = {1, 2, 3};     // ID = 0
    vector<int> data1 = {4, 5, 6};     // ID = 1
    vector<int> data2 = {7, 8, 9};     // ID = 2
    vector<int> data3 = {10, 11, 12};  // ID = 3
    graph.addVertex(data0);
    graph.addVertex(data1);
    graph.addVertex(data2);
    graph.addVertex(data3);
    graph.addEdge(0, 1, graph.euclideanDistance(data0, data1));
    graph.addEdge(0, 2, graph.euclideanDistance(data0, data2));
    graph.addEdge(0, 3, graph.euclideanDistance(data0, data3));
    graph.addEdge(1, 2, graph.euclideanDistance(data1, data2));

    vector<int> neighbors0 = graph.getOutNeighbors(0);    // Έλεγχος των γειτόνων του κόμβου 0 (περίπτωση με πολλούς γείτονες)
    TEST_ASSERT(neighbors0.size() == 3);
    TEST_ASSERT(std::find(neighbors0.begin(), neighbors0.end(), 1) != neighbors0.end());
    TEST_ASSERT(std::find(neighbors0.begin(), neighbors0.end(), 2) != neighbors0.end());
    TEST_ASSERT(std::find(neighbors0.begin(), neighbors0.end(), 3) != neighbors0.end());

    vector<int> neighbors1 = graph.getOutNeighbors(1);    // Έλεγχος των γειτόνων του κόμβου 1
    TEST_ASSERT(neighbors1.size() == 1);
    TEST_ASSERT(neighbors1[0] == 2);

    vector<int> neighbors3 = graph.getOutNeighbors(3);    // Έλεγχος των γειτόνων του κόμβου 3 (κόμβος χωρίς γείτονες)
    TEST_ASSERT(neighbors3.size() == 0);

    // Έλεγχος για μη υπαρκτό κόμβο
    try {
        vector<int> neighbors_invalid = graph.getOutNeighbors(100);
        TEST_ASSERT(neighbors_invalid.size() == 0); // Αν η συνάρτηση επιστρέφει κενό vector
    } catch (const std::out_of_range& e) {
        TEST_ASSERT(true);
    } catch (...) {
        TEST_ASSERT(false);
    }
}

void test_initializeRandomGraph(){
    int R = 5;
    int numVertices = 100;
    Graph<int> graph({}, R, 3, 0.5, 10);
    mt19937 rng(42);
    uniform_int_distribution<int> dist(-100, 100);
    for (int i = 0; i < numVertices; ++i) {
        vector<int> point = {dist(rng), dist(rng)};
        graph.addVertex(point);
    }

    // Αρχικοποίηση τυχαίου γραφήματος
    graph.initializeRandomGraph(R);

    // Έλεγχος για το αν τηρείται το όριο γειτόνων R
    const auto& vertexMap = graph.getVertexMap();
    for (const auto& [vertex, neighbors] : vertexMap) {
        vector<int> outNeighbors = graph.getOutNeighbors(vertex);
        TEST_ASSERT(outNeighbors.size() <= R);
    }

    // Έλεγχος για self-loops
    for (const auto& [vertex, neighbors] : vertexMap) {
        vector<int> outNeighbors = graph.getOutNeighbors(vertex);
        for (int neighbor : outNeighbors) {
            TEST_ASSERT(neighbor != vertex);
        }
    }

    // Έλεγχος της τυχαιότητας με επανάληψη
    map<int, vector<int>> neighbors_snapshot;
    for (const auto& [vertex, neighbors] : vertexMap) {
        neighbors_snapshot[vertex] = graph.getOutNeighbors(vertex);
    }

    // Επανεκτέλεση της `initializeRandomGraph` και έλεγχος ότι οι γείτονες αλλάζουν
    graph.initializeRandomGraph(R);
    bool neighbors_changed = false;
    for (const auto& [vertex, old_neighbors] : neighbors_snapshot) {
        const auto& new_neighbors = graph.getOutNeighbors(vertex);
        if (new_neighbors != old_neighbors) {
            neighbors_changed = true;
            break;
        }
    }
    TEST_ASSERT(neighbors_changed);
}


void test_findMedoid() {
    Graph<int> graph0({}, 5, 3, 0.5, 10);
    TEST_ASSERT(graph0.findMedoid() == -1);        // Έλεγχος αν το medoid είναι το αναμενόμενο


    Graph<int> graph1({}, 5, 3, 0.5, 10);
    graph1.addVertex({0, 0}); // ID 0
    graph1.addVertex({2, 0}); // ID 1
    graph1.addVertex({0, 2}); // ID 2
    graph1.addVertex({2, 2}); // ID 3
    graph1.addVertex({1, 1}); // ID 4 - Αναμενόμενο medoid

    TEST_ASSERT(graph1.findMedoid() == 4);        // Έλεγχος αν το medoid είναι το αναμενόμενο


    Graph<int> graph2({}, 5, 3, 0.5, 10);    // Προσθήκη κορυφών με τα ίδια δεδομένα
    graph2.addVertex({1, 1}); // ID 0
    graph2.addVertex({1, 1}); // ID 1
    graph2.addVertex({1, 1}); // ID 2

    int medoid_id2 = graph2.findMedoid();
    TEST_ASSERT(medoid_id2 >= 0 && medoid_id2 <= 2);    // Ελέγχουμε ότι το medoid_id είναι έγκυρο (0, 1 ή 2)


    Graph<int> graph3({}, 5, 3, 0.5, 10);// Προσθήκη μονοδιάστατων σημείων
    graph3.addVertex({1}); // ID 0
    graph3.addVertex({2}); // ID 1
    graph3.addVertex({3}); // ID 2
    graph3.addVertex({4}); // ID 3
    graph3.addVertex({5}); // ID 4

    TEST_ASSERT(graph3.getVertexData(graph3.findMedoid())[0] == 3);    // Αναμενόμενο medoid είναι το μεσαίο σημείο (ID 2)

    Graph<int> graph4({}, 5, 3, 0.5, 10);
    graph4.addVertex({1, 2}); // ID 0
    graph4.addVertex({2, 1}); // ID 1
    graph4.addVertex({4, 2}); // ID 2
    graph4.addVertex({5, 3}); // ID 3
    graph4.addVertex({6, 4}); // ID 4 - Αναμενόμενο medoid
    graph4.addVertex({7, 5}); // ID 5
    graph4.addVertex({8, 6}); // ID 6
    graph4.addVertex({9, 7}); // ID 7
    graph4.addVertex({10, 8}); // ID 8
    graph4.addVertex({11, 9}); // ID 9
    graph4.addVertex({12, 10}); // ID 10
    graph4.addVertex({3, 4}); // ID 11
    graph4.addVertex({4, 2}); // ID 12

    TEST_ASSERT(graph4.findMedoid() == 4);        // Έλεγχος αν το medoid είναι το αναμενόμενο


}

void test_findMedoidSample() {
    Graph<int> graph({}, 5, 3, 0.5, 10);
    int num_points = 10000; // Αριθμός σημείων (μεγαλύτερο από 1000)
    mt19937 rng(42); // Σταθερός seed για αναπαραγωγή
    uniform_int_distribution<int> dist(-100, 100);

    // Προσθήκη τυχαίων σημείων στον γράφο
    for (int i = 0; i < num_points; ++i) {
        vector<int> point = {dist(rng), dist(rng)};
        graph.addVertex(point);
    }

    // Εύρεση του medoid χρησιμοποιώντας τη findMedoidSample
    int medoid_sample_id1 = graph.findMedoidSample(num_points / 10);
    double sample_distance1 = numeric_limits<double>::infinity();

    int medoid_sample_id2 = graph.findMedoidSample(num_points / 10);
    double sample_distance2 = numeric_limits<double>::infinity();

    // Έλεγχος ότι το medoid_id είναι έγκυρο
    TEST_ASSERT(medoid_sample_id1 >= 0 && medoid_sample_id1 < num_points);
    TEST_ASSERT(medoid_sample_id2 >= 0 && medoid_sample_id2 < num_points);

    int medoid_id = -1;
    double min_total_distance = numeric_limits<double>::infinity();

    const auto& vertex_map = graph.getVertexMap();
    // Διατρέχουμε όλα τα σημεία για να υπολογίσουμε το medoid
    for (const auto& [id_i, vec_i] : vertex_map) {
        double total_distance = 0.0;
        for (const auto& [id_j, vec_j] : vertex_map) {
            if (id_i != id_j) {
                total_distance += graph.euclideanDistance(vec_i, vec_j);
            }
        }
        if (id_i == medoid_sample_id1) {
            sample_distance1 = total_distance;
        }
        if (id_i == medoid_sample_id2) {
            sample_distance2 = total_distance;
        }
        if (total_distance < min_total_distance) {
            min_total_distance = total_distance;
            medoid_id = id_i;
        }
    }
    double tolerance = 0.01 * min_total_distance;  // Όριο ανοχής στο 1% της ελάχιστης συνολικής απόστασης

    if (fabs(sample_distance1 - min_total_distance) >= tolerance) {
        std::cout << "\nSample Medoid Distance: " << sample_distance1 << ", Full Medoid Distance: " << min_total_distance << std::endl;
    }
    TEST_ASSERT(fabs(sample_distance1 - min_total_distance) < tolerance);

    if (fabs(sample_distance2 - min_total_distance) >= tolerance) {
        std::cout << "\nSample Medoid Distance: " << sample_distance2 << ", Full Medoid Distance: " << min_total_distance << std::endl;
    }
    TEST_ASSERT(fabs(sample_distance2 - min_total_distance) < tolerance);
}

void test_setOutNeighbors() {
    Graph<int> graph({}, 5, 3, 0.5, 10);
    graph.addVertex({1, 2});    // ID 0
    graph.addVertex({3, 4});    // ID 1
    graph.addVertex({5, 6});    // ID 2
    graph.addVertex({7, 8});    // ID 3

    graph.setOutNeighbors(0, {1, 2});
    auto edges = graph.getEdges(0);
    TEST_ASSERT(edges.size() == 2);      // Πρέπει να υπάρχουν 2 ακμές

    // Έλεγχος των δεδομένων κάθε ακμής
    TEST_ASSERT(edges[0].getDestination() == 1);
    TEST_ASSERT(edges[1].getDestination() == 2);

    // Έλεγχος ότι οι παλιοί γείτονες έχουν διαγραφεί και ο νέος γείτονας έχει προστεθεί
    graph.setOutNeighbors(0, {3});
    edges = graph.getEdges(0);
    TEST_ASSERT(edges.size() == 1);             // Πρέπει να υπάρχει μόνο 1 ακμή
    TEST_ASSERT(edges[0].getDestination() == 3);// και να είναι ο 3
}

void test_addOutNeighbor() {
    Graph<int> graph({}, 5, 3, 0.5, 10);
    graph.addVertex({1, 2});    // ID 0
    graph.addVertex({3, 4});    // ID 1
    graph.addVertex({5, 6});    // ID 2
    graph.addVertex({7, 8});    // ID 3

    graph.addOutNeighbor(0, 1);
    auto edges = graph.getEdges(0);
    TEST_ASSERT(edges.size() == 1);
    TEST_ASSERT(edges[0].getDestination() == 1);
    double expectedDist01 = graph.euclideanDistance({1, 2}, {3, 4});
    TEST_ASSERT(edges[0].getWeight() == expectedDist01);

    graph.addOutNeighbor(0, 1);
    edges = graph.getEdges(0);
    TEST_ASSERT(edges.size() == 1);
    graph.addOutNeighbor(0, 2);
    edges = graph.getEdges(0);
    TEST_ASSERT(edges.size() == 2);
    TEST_ASSERT(edges[1].getDestination() == 2);
    double expectedDist02 = graph.euclideanDistance({1, 2}, {5, 6});
    TEST_ASSERT(edges[1].getWeight() == expectedDist02);
}

void test_euclideanDistance() {
    vector<int> v1 = {1, 2};
    vector<int> v2 = {4, 6};
    double distance = Graph<int>::euclideanDistance(v1, v2);
    TEST_ASSERT(fabs(distance - 5.0) < 1e-6); // Αναμένουμε απόσταση 5.0
}

void test_greedySearch() {
    TEST_ASSERT(false);
}

void test_robustPrune() {
    TEST_ASSERT(false);
}

void test_vamanaIndexing() {
    TEST_ASSERT(false);
}


// Λίστα με όλα τα tests
TEST_LIST = {
    { "addVertex", test_addVertex },
    { "addEdge", test_addEdge },
    { "getOutNeighbors", test_getOutNeighbors },
    { "initializeRandomGraph", test_initializeRandomGraph },
    { "findMedoid", test_findMedoid },
    { "findMedoidSample", test_findMedoidSample },
    { "setOutNeighbors", test_setOutNeighbors },
    { "addOutNeighbor", test_addOutNeighbor },
    { "euclideanDistance", test_euclideanDistance },
    { "greedySearch", test_greedySearch },
    { "robustPrune", test_robustPrune },
    { "vamanaIndexing", test_vamanaIndexing },
    { NULL, NULL } // Τερματισμός της λίστας
};
