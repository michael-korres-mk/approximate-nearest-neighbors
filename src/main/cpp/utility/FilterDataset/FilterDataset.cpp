//
// Created by mkorres on 11/10/2024.
//
#include "../Utils/Utils.h"
#include "FilterDataset.h"

#include <cstdint>

template<typename T>
FilterDataset<T>::FilterDataset() {}

template<typename T>
FilterDataset<T>::FilterDataset(const string &dataFileName) {
    dataPoints = vecsRead(dataFileName);
    assert(!dataPoints.empty());
    numOfDataPoints = dataPoints.size();
}

template<typename T>
vector<DataPoint<T>> FilterDataset<T>::vecsRead(const string &filename) {

    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);


    // Read the vector size (dimension)
    uint32_t numOfDataPoints;
    file.read(reinterpret_cast<char*>(&numOfDataPoints), sizeof(int));

    // Read the vectors
    vector<DataPoint<T>> dataPoints = vector<DataPoint<T>>(numOfDataPoints);
    T data;

    for (int i = 0; i < numOfDataPoints; ++i) {
        dataPoints[i] = DataPoint<T>(-1,-1,vector<T>(FILTER_DATASET_DIMENSION - 2));

        float C;
        file.read(reinterpret_cast<char*>(&C), sizeof(float));

        float timestamp;
        file.read(reinterpret_cast<char*>(&timestamp), sizeof(float));

        dataPoints[i].C = static_cast<int>(C);
        dataPoints[i].T = timestamp;

        for(int j = 0; j < FILTER_DATASET_DIMENSION - 2; j++) {
            file.read(reinterpret_cast<char*>(&data),sizeof(T));
            dataPoints[i].vec[j] = data;
        }

    }

    file.close();
    return dataPoints;

}


template <typename T>
void FilterDataset<T>::print() {

    cout << "d = " << FILTER_DATASET_DIMENSION - 2 << endl;
    cout << "numOfDataPoints = " << numOfDataPoints << endl;
    cout << "sizeof(T) = " << sizeof(T) << endl;

    for(int i = 0; i < numOfDataPoints; i++) {
        fprintf(stdout, "\e[36mvector[ %d ] is:   \e[0m\n",i);
        dataPoints[i].print();
    }
}

// explicit template instantiations
template class FilterDataset<float>;
template class FilterDataset<int>;
template class FilterDataset<char>;