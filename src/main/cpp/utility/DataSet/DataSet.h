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

#include "../Edge/Edge.h"
#include "../Utils/Utils.h"
#include "../DataPoint/DataPoint.h"

using namespace std;

template <typename T>
class DataSet {
public:
    int d;
    int numOfDatapoints;
    vector<DataPoint<T>> datapoints;

    explicit DataSet();
    explicit DataSet(const string& filename);

    DataPoint<T> readDataPoint(ifstream &file, int id,int& globalDims);

    void print();
    static void vecsWrite(const string &filename, const vector<vector<T>> &vectors,int globalDim);
};



#endif //DATASET_H
