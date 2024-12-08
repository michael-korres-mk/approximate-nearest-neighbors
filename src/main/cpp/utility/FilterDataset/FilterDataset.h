//
// Created by mkorres on 11/10/2024.
//

#ifndef FILTERDATASET_H
#define FILTERDATASET_H

#include "../../utility/DataPoint/DataPoint.h"
#include "../../utility/Utils/Utils.h"

#include <map>

#include <cassert>
#include <cstdint>
#include <fstream>
#include <cstdint>


template <typename T>
class FilterDataset {
public:
    int numOfDataPoints;
    map<int,vector<int>> datapointGroups;
    vector<DataPoint<T>> datapoints;

    explicit FilterDataset();
    explicit FilterDataset(const string& dataFileName);

    DataPoint<T> readDataPoint(ifstream &file, int id);

    void print();
};



#endif //FILTERDATASET_H
