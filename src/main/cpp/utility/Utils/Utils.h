//
// Created by micha on 09/10/2024.
//

#ifndef OS_PROJECT_1_UTILS_H
#define OS_PROJECT_1_UTILS_H

#include <iostream>
#include <cstdlib>

#define BUFFER_SIZE 1024
#define LAST_NAME_LEN 20
#define FIRST_NAME_LEN 20
#define ZIP_CODE_LEN 6
#define INT_LEN 128

#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

class Utils {
public:
    static int flipCoin();
    static vector<vector<float>> fvecs_read(const string& filename, pair<int, int> bounds = {1, -1}) ;

};


#endif //OS_PROJECT_1_UTILS_H
