//
// Created by mkorres on 11/10/2024.
//

#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <vector>
#include <iomanip>
#include "../../utility/Utils/Utils.h"

using namespace std;

template <typename K>
class DataPoint {
public:
    int id;
    int C;
    int T;
    vector<K> vec;
    explicit DataPoint() ;
    explicit DataPoint(int id,int C, int T,vector<K> vector) ;
    void print();
};



#endif //DATAPOINT_H
