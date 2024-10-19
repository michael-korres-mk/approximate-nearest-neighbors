//
// Created by micha on 09/10/2024.
//

# include "Utils.h"
#include <chrono>
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <random>
# include <unistd.h>

using namespace std;

template <typename T>
void Utils<T>::shuffle(vector<vector<T>>& v){
    const unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    std::shuffle(v.begin(), v.end(), rng);
}

template <typename T>
int Utils<T>::flipCoin() { // Function to simulate a coin flip
    // seed engine
    random_device seed;

    // Random number engine
    mt19937 generator(seed());

    // Set the range => {0, 1}
    uniform_int_distribution<int> distribution(0, 1);

    // Generate a random integer (0 or 1)
    return distribution(generator);

}

template class Utils<float>;
template class Utils<int>;
template class Utils<char>;