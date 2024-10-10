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
template <typename T>
vector<vector<T>> Utils::vecs_read(const string& filename, pair<int, int> bounds) {

    char dataFilePath[BUFFER_SIZE] = {'\0' };

    strcat(dataFilePath,SOURCE_D);strcat(dataFilePath,MAIN_D);strcat(dataFilePath,RESOURCES_D);strcat(dataFilePath,filename.c_str());

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // Read the vector size (dimension)
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));
    int vec_sizeof = sizeof(int) + d * sizeof(T);

    cout << "d: " << d << endl;

    cout << "vec_sizeof: " << vec_sizeof << endl;

    // Find total number of vectors in the file
    file.seekg(0, ios::end);
    streampos file_size = file.tellg();
    int bmax = file_size / vec_sizeof;

    cout << "bmax: " << bmax << endl;

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
    vector<vector<T>> vectors(n, vector<T>(d));

    for (int i = 0; i < n; ++i) {
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int)); // Read the dimension (should be d)

        // Read the vector components
        file.read(reinterpret_cast<char*>(vectors[i].data()), d * sizeof(T));

        // Check if the dimension read matches the expected dimension
        assert(dim == d);
    }

    file.close();
    return vectors;
}

// explicit template instantiations
template vector<vector<int>> Utils::vecs_read(const string& filename, pair<int, int> bounds);
template vector<vector<char>> Utils::vecs_read(const string& filename, pair<int, int> bounds);
template vector<vector<float>> Utils::vecs_read(const string& filename, pair<int, int> bounds);