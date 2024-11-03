//
// Created by mkorres on 11/3/2024.
//

#ifndef SORTEDCONTAINER_H
#define SORTEDCONTAINER_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class VamanaContainer {
private:
    vector<pair<int,double>> items;
    size_t maxSize;
public:
    VamanaContainer(size_t L);
    void insert(const pair<int,double>& newItem);
    bool contains(int id) const;
    vector<int> subset(int k);
    void print() const ;
};



#endif //SORTEDCONTAINER_H
