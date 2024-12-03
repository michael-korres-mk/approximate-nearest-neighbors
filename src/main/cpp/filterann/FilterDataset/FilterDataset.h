//
// Created by mkorres on 11/10/2024.
//

#ifndef FILTERDATASET_H
#define FILTERDATASET_H

#include "../DataPoint/DataPoint.h"
#include "../Query/Query.h"
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
    map<int,vector<DataPoint<T>>> datapoints;

    explicit FilterDataset();
    explicit FilterDataset(const string& dataFileName);

    vector<DataPoint<int>> getNearestNeighbors(const Query<T> &q, const int &k);

    void print();
};



#endif //FILTERDATASET_H
