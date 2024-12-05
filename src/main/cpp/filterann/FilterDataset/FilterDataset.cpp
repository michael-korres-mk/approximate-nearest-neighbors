//
// Created by mkorres on 11/10/2024.
//
#include "FilterDataset.h"

template<typename T>
FilterDataset<T>::FilterDataset():numOfDataPoints(0) {}

template<typename T>
FilterDataset<T>::FilterDataset(const string &filename):numOfDataPoints(0) {

    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);


    // Read the vector size (dimension)
    uint32_t numOfDataPoints;
    file.read(reinterpret_cast<char*>(&numOfDataPoints), sizeof(int));

    for (uint32_t i = 0; i < numOfDataPoints; ++i) {

        DataPoint<T> dataPoint = readDataPoint(file,i);
        int C = dataPoint.C;

        if(datapointGroups.find(C) == datapointGroups.end()) datapointGroups.insert(make_pair(C,vector<int>()));

        datapointGroups[C].push_back(dataPoint.id);
        datapoints.push_back(dataPoint);
    }

    file.close();

    this->numOfDataPoints = static_cast<int>(numOfDataPoints);

    assert(this->numOfDataPoints > 0);

}

template<typename T>
DataPoint<T> FilterDataset<T>::readDataPoint(ifstream& file,int id) {
    // Read the vectors
    T data;
    vector<T> vec(FILTER_DATASET_DIMENSION - 2);

    float C;
    file.read(reinterpret_cast<char*>(&C), sizeof(float));

    float timestamp;
    file.read(reinterpret_cast<char*>(&timestamp), sizeof(float));

    for(int j = 0; j < FILTER_DATASET_DIMENSION - 2; j++) {
        file.read(reinterpret_cast<char*>(&data),sizeof(T));
        vec[j] = data;
    }

    return DataPoint<T>(id,static_cast<int>(C),timestamp,vec);;
}


template <typename T>
void FilterDataset<T>::print() {

    cout << "d = " << FILTER_DATASET_DIMENSION - 2 << endl;
    cout << "numOfDataPoints = " << numOfDataPoints << endl;
    cout << "sizeof(T) = " << sizeof(T) << endl;

    for(int i = 0; i < numOfDataPoints; i++) {
        printf("vector[ %d ] is: \n",i);
    }
}

// explicit template instantiations
template class FilterDataset<float>;
template class FilterDataset<int>;
template class FilterDataset<char>;