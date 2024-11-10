//
// Created by mkorres on 11/10/2024.
//

#include "DataPoint.h"

#include "../Utils/Utils.h"

template<typename K>
DataPoint<K>::DataPoint() {}

template<typename K>
DataPoint<K>::DataPoint(const int C, const int T, vector<K> vector): C(C), T(T), vec(vector) {}

template<typename K>
void DataPoint<K>::print() {

    cout << "C = " << C << endl;
    cout << "T = " << T << endl;
    for(int j = 0; j < FILTER_DATASET_DIMENSION - 1; j++) {
        cout << vec[j] << " ";
    }
    cout << setw(4) << fixed << setprecision(4) << vec[FILTER_DATASET_DIMENSION - 1] << endl;

}

// explicit template instantiations
template class DataPoint<float>;
template class DataPoint<int>;
template class DataPoint<char>;