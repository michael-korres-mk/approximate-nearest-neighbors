//
// Created by mkorres on 11/3/2024.
//

#include "VamanaContainer.h"

#include <iostream>
#include <vector>
#include <algorithm>

VamanaContainer::VamanaContainer(size_t L) : maxSize(L) {}

void VamanaContainer::insert(const pair<int,double>& newItem) {

    const auto& [newVertex,newDistance] = newItem;

    if(contains(newVertex)) return;

    if(items.size() == maxSize && newDistance > items.back().second) return;

    items.push_back(newItem);

    sort(items.begin(), items.end(), [](const pair<int,double>& a, const pair<int,double>& b) {return a.second < b.second;});

    if (items.size() > maxSize) {
        items.pop_back(); // Remove the largest item
    }
}

bool VamanaContainer:: contains(int id) const {
    return any_of(items.begin(), items.end(), [id](const auto& item) {return item.first == id;});
}

vector<int> VamanaContainer::subset(int k) {
    // Αποθήκευση των k πιο κοντινών γειτόνων
    vector<int> subset;
    int itemSize = static_cast<int>(items.size());
    for (int i = 0; (k == -1 || i < k) && i < itemSize; i++) {
        subset.emplace_back(items[i].first);
    }
    return subset;
}

void VamanaContainer:: print() const {
    for (const auto& item : items) {
        std::cout << item.first << " ";
    }
    std::cout << std::endl;
}
