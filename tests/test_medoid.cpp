#include "acutest.h"
#include "Graph/Graph.h"
#include <vector>
#include <random>

void test_medoid() {
    Graph<int> graph({}, 1, 5, 3, 1.2);
    int num_points = 10000; // Αριθμός σημείων (μεγαλύτερο από 1000)
    mt19937 rng(42); // Σταθερός seed για αναπαραγωγή
    uniform_int_distribution<int> dist(-100, 100);

    // arrange
    // Προσθήκη τυχαίων σημείων στον γράφο
    for (int i = 0; i < num_points; ++i) {
        vector<int> point = {dist(rng), dist(rng)};
        graph.addVertex(point);
    }

    // act
    // Εύρεση του medoid δύο φορές για να εξασφαλίσω ότι δεν είναι τυχαίο
    int medoid_sample_id1 = graph.medoid();
    double sample_distance1 = numeric_limits<double>::infinity();

    int medoid_sample_id2 = graph.medoid();
    double sample_distance2 = numeric_limits<double>::infinity();

    // 1st assert
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

    // 2st assert
    if (fabs(sample_distance1 - min_total_distance) >= tolerance) {
        std::cout << "\nSample Medoid Distance: " << sample_distance1 << ", Full Medoid Distance: " << min_total_distance << std::endl;
    }
    TEST_ASSERT(fabs(sample_distance1 - min_total_distance) < tolerance);

    if (fabs(sample_distance2 - min_total_distance) >= tolerance) {
        std::cout << "\nSample Medoid Distance: " << sample_distance2 << ", Full Medoid Distance: " << min_total_distance << std::endl;
    }
    TEST_ASSERT(fabs(sample_distance2 - min_total_distance) < tolerance);
}

TEST_LIST = {
    { "Medoid", test_medoid },
    { NULL, NULL }
};
