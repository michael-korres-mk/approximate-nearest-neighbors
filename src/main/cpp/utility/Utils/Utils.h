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
#define SOURCE_D "src/"
#define MAIN_D "main/"
#define RESOURCES_D "resources/"

using namespace std;

class Utils {
public:
    static int flipCoin();

    template<typename T>
    static vector<vector<T>> vecs_read(const string &filename, pair<int, int> bounds);
};


#endif //OS_PROJECT_1_UTILS_H
