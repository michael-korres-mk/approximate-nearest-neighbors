//
// Created by mkorres on 10/9/2024.
//

#ifndef DATASET_H
#define DATASET_H

# include <iomanip>
# include <valarray>
# include <vector>
# include <cassert>
# include <fstream>
#include <map>
# include "../Edge/Edge.h"
using namespace std;

template <typename T>
class DataSet {
    int d;
    int numOfVectors;
    vector<vector<T>> vectors;
public:

    // instance
    explicit DataSet();
    explicit DataSet(const string& dataFileName);
    int getD();
    int getNumOfVectors();
    vector<vector<T>> getVectors();
    vector<T> getVector(int id);
    vector<vector<T>> vecsRead(const string& filename, pair<int, int> bounds);
    void print();
};



#endif //DATASET_H
