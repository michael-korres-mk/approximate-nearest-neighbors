//
// Created by mkorres on 10/9/2024.
//

#ifndef DATASET_H
#define DATASET_H

# include <iomanip>
#include <unistd.h>
# include <valarray>
# include <vector>
# include <cassert>
# include <fstream>
#include <map>
#include <chrono>

#include "../../utility/Edge/Edge.h"
#include "../../utility/Utils/Utils.h"

using namespace std;

template <typename T>
class DataSet {
public:
    int d;
    int numOfVectors;
    vector<vector<T>> vectors;

    // instance
    explicit DataSet();
    explicit DataSet(const string& dataFileName);
    static void vecsWrite(const string &filename, const vector<vector<T>> &vectors,int globalDim);
    void print();
};



#endif //DATASET_H
