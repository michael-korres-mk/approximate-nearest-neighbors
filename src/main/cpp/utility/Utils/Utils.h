//
// Created by micha on 09/10/2024.
//

#ifndef ANN_UTILS_H
#define ANN_UTILS_H

#include <chrono>
# include <iostream>
#include <vector>
#include <sys/time.h>

#define BUFFER_SIZE 1024

#define SOURCE_D "src/"
#define MAIN_D "main/"
#define RESOURCES_D "resources/"
#define RESOURCES_P "src/main/resources/"

#define FILTER_DATASET_DIMENSION 102
#define FILTER_QUERY_SET_DIMENSION 104


#define TIMER_BLOCK(MSG, CODE) { \
    auto start = chrono::high_resolution_clock::now(); \
    CODE \
    auto finish = chrono::high_resolution_clock::now(); \
    auto datasetDuration = chrono::duration_cast<chrono::milliseconds>(finish - start).count(); \
    cout << MSG << ": " << datasetDuration << " ms" << endl; \
}

#define PRINT_VAR(var) cout << #var << " = " << var << endl;

#define GET_INT_ARG(arg_name, variable)   \
    if (strcmp(argv[i], arg_name) == 0) { \
        variable = atoi(argv[i + 1]);    \
    }

#define GET_DOUBLE_ARG(arg_name, variable)  \
    if (strcmp(argv[i], arg_name) == 0) {  \
        variable = atof(argv[i + 1]);     \
    }

#define GET_STRING_ARG(arg_name, variable)  \
if (strcmp(argv[i], arg_name) == 0) {  \
    variable = argv[i + 1];     \
}

using namespace std;

template <typename  K>
class Utils {
public:
    static vector<K>& shuffle(vector<K>& v);
    static int flipCoin();
    static int random(int a,int b);
    static double euclideanDistance(const vector<K>& v1,const vector<K>& v2);
    static void printDivider();
    static void printVec(vector<K> v);
};

#endif //ANN_UTILS_H
