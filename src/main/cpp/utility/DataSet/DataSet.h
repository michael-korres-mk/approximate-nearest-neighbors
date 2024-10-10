//
// Created by mkorres on 10/9/2024.
//

#ifndef DATASET_H
#define DATASET_H

#include <iomanip>
#include <valarray>
#include <vector>
#include <cassert>
#include <fstream>

using namespace std;

template <typename T>
class DataSet {
    int d;
    int numOfVectors;
    vector<vector<T>> vectors;
public:

    explicit DataSet(char* dataFileName);
    int getD();
    int getNumOfVectors();
    vector<vector<T>> getVectors();
    vector<T> getVector(int id);
    vector<vector<T>> vecsRead(const string& filename, pair<int, int> bounds);
    static float euclideanDistance(vector<T> v1,vector<T> v2);
    int getNearestNeighbor(vector<T> q);
    void print();
};



#endif //DATASET_H
