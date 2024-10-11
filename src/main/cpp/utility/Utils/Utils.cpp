//
// Created by micha on 09/10/2024.
//

# include "Utils.h"
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <random>
# include <unistd.h>

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