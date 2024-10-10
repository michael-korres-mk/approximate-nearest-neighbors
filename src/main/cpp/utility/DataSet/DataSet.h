//
// Created by mkorres on 10/9/2024.
//

#ifndef DATASET_H
#define DATASET_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
# include <fcntl.h>
#include "../Utils/Utils.h"


using namespace std;

template <typename T>
class DataSet {
    int d;
    int numOfVectors;
    T** vectors;
public:

    explicit DataSet(char* dataFileName);
    ~DataSet();
    int getD();
    int getNumOfVectors();
    T** getVectors();
    void print();
};



#endif //DATASET_H
