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

template <typename K>
vector<K>& Utils<K>::shuffle(vector<K>& v){
    const unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 rng(seed);
    std::shuffle(v.begin(), v.end(), rng);
    return v;
}

template <typename K>
int Utils<K>::flipCoin() { // Function to simulate a coin flip
   return random(0,1);

}

template <typename K>
int Utils<K>::random(int a,int b) { // Function to simulate a coin flip
    // seed engine
    random_device seed;

    // Random number engine
    mt19937 generator(seed());

    // Set the range => {a, b}
    uniform_int_distribution<int> distribution(a, b);

    // Generate a random integer
    return distribution(generator);

}

template class Utils<float>;
template class Utils<int>;
template class Utils<char>;
template class Utils<vector<float>>;
template class Utils<vector<int>>;
template class Utils<vector<char>>;