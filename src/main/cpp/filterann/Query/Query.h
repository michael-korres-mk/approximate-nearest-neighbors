//
// Created by mkorres on 11/11/2024.
//

#ifndef QUERY_H
#define QUERY_H

#include "../../utility/Utils/Utils.h"

#include <vector>
#include <iomanip>

using namespace std;

template <typename K>
class Query {
public:
    int queryType;
    int v;
    float l;
    float r;
    vector<K> vec;
    explicit Query() ;
    explicit Query(int queryType,int v,float l,float r,vector<K> vec) ;
    void print();
};



#endif //QUERY_H
