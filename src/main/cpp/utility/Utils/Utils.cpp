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
    random_device seed;

    // Random number engine
    mt19937 generator(seed());

    // Set the range => {0, 1}
    uniform_int_distribution<int> distribution(0, 1);

    // Generate a random integer (0 or 1)
    return distribution(generator);

}