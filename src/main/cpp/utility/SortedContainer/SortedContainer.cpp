//
// Created by mkorres on 11/3/2024.
//

#include "SortedContainer.h"

#include <iostream>
#include <vector>
#include <algorithm>

SortedContainer::SortedContainer(size_t L) : maxSize(L) {}

void SortedContainer::insert(const pair<int,double>& newItem) {

    const auto& [newVertex,newDistance] = newItem;

    if(items.size() == maxSize && newDistance > items.back().second) return;

    items.push_back(newItem);

    sort(items.begin(), items.end(), [](const pair<int,double>& a, const pair<int,double>& b) {return a.second < b.second;});

    if (items.size() > maxSize) {
        items.pop_back(); // Remove the largest item
    }
}

bool SortedContainer:: contains(int id) const {
    for(const auto& [itemId,_] : items ) {
        if(itemId == id) return true;
    }
    return false;
}

vector<int> SortedContainer::subset(int k) {
    // Αποθήκευση των k πιο κοντινών γειτόνων
    vector<int> subset;
    for (int i = 0; (i == -1 || i < k) && i < items.size(); i++) {
        subset.emplace_back(items[i].first);
    }
    return subset;
}

void SortedContainer:: print() const {
    for (const auto& item : items) {
        std::cout << item.first << " ";
    }
    std::cout << std::endl;
}
