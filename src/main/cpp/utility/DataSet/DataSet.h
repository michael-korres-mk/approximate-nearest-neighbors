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

using namespace std;

class DataSet {
    int d;
    int numOfVectors;
    float** vectors;
public:
    DataSet();

    DataSet(char *dataFileName);

    DataSet(int d, int numOfVectors,float** vectors);
    int getD();
    int getNumOfVectors();
    float** getVectors();
    void print();
};



#endif //DATASET_H
