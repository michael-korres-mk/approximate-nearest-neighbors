//
// Created by mkorres on 11/10/2024.
//

#ifndef FILTERDATASET_H
#define FILTERDATASET_H

#include "../DataPoint/DataPoint.h"
#include "../DataSet/DataSet.h"

template <typename T>
class FilterDataset {
public:
    int numOfDataPoints;
    vector<DataPoint<T>> dataPoints;

    explicit FilterDataset();
    explicit FilterDataset(const string& dataFileName);
    vector<DataPoint<T>> vecsRead(const string& filename);
    void print();
};



#endif //FILTERDATASET_H
