//
// Created by mkorres on 12/3/2024.
//

#ifndef FILTERDATASETDIVIDER_H
#define FILTERDATASETDIVIDER_H

#include "../../filterann/FilterDataset/FilterDataset.h"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <filesystem> // (C++17)

template <typename T>
class InputSetDivider {
public:
    static void divideFilterDataset(const string& filename);
};



#endif //FILTERDATASETDIVIDER_H
