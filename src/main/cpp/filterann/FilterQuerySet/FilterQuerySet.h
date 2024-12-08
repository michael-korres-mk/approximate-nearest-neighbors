//
// Created by mkorres on 11/11/2024.
//

#ifndef FILTERQUERYSET_H
#define FILTERQUERYSET_H

#include "../Query/Query.h"
#include "../../utility/Utils/Utils.h"
#include "FilterQuerySet.h"

#include <cstdint>
#include <fstream>
#include <cassert>

template <typename T>
class FilterQuerySet {
public:
    int numOfQueries;
    vector<Query<T>> queries;

    explicit FilterQuerySet();
    explicit FilterQuerySet(const string& dataFileName);
    vector<Query<T>> vecsRead(const string& filename);
    void print();
};



#endif //FILTERQUERYSET_H
