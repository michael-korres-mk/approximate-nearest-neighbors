//
// Created by micha on 09/10/2024.
//

#include "Utils.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <random>
#include <unistd.h>

using namespace std;

int Utils::flipCoin() { // Function to simulate a coin flip
    // seed engine
    std::random_device seed;

    // Random number engine
    std::mt19937 generator(seed());

    // Set the range => {0, 1}
    std::uniform_int_distribution<int> distribution(0, 1);

    // Generate a random integer (0 or 1)
    return distribution(generator);

}

// GPT generated for checking | Equivalent of given Matlab function
vector<vector<float>> Utils::fvecs_read(const string& filename, pair<int, int> bounds) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // Read the vector size (dimension)
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));
    int vec_sizeof = sizeof(int) + d * sizeof(float);

    // Find total number of vectors in the file
    file.seekg(0, ios::end);
    streampos file_size = file.tellg();
    int bmax = file_size / vec_sizeof;

    int a = bounds.first;
    int b = (bounds.second == -1) ? bmax : bounds.second;

    // Ensure the bounds are valid
    assert(a >= 1);
    if (b > bmax) b = bmax;
    if (b == 0 || b < a) return {};

    // Calculate the number of vectors to read
    int n = b - a + 1;

    // Seek to the starting position
    file.seekg((a - 1) * vec_sizeof, ios::beg);

    // Read the vectors
    vector<vector<float>> vectors(n, vector<float>(d));

    for (int i = 0; i < n; ++i) {
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int)); // Read the dimension (should be d)

        // Read the vector components
        file.read(reinterpret_cast<char*>(vectors[i].data()), d * sizeof(float));

        // Check if the dimension read matches the expected dimension
        assert(dim == d);
    }

    file.close();
    return vectors;
}

