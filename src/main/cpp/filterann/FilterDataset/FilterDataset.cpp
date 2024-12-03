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

    // Read the vectors
    T data;
    DataPoint<T> dataPoint;

    for (int i = 0; i < numOfDataPoints; ++i) {
        dataPoint = DataPoint<T>(-1,-1,vector<T>(FILTER_DATASET_DIMENSION - 2));

        float C;
        file.read(reinterpret_cast<char*>(&C), sizeof(float));

        float timestamp;
        file.read(reinterpret_cast<char*>(&timestamp), sizeof(float));

        dataPoint.C = static_cast<int>(C);
        dataPoint.T = timestamp;

        for(int j = 0; j < FILTER_DATASET_DIMENSION - 2; j++) {
            file.read(reinterpret_cast<char*>(&data),sizeof(T));
            dataPoint.vec[j] = data;
        }

        if(datapoints.find(C) == datapoints.end()) datapoints.insert(make_pair(C,vector<DataPoint<T>>()));

        datapoints[C].push_back(dataPoint);
    }

    file.close();

    this->numOfDataPoints = static_cast<int>(numOfDataPoints);

    assert(this->numOfDataPoints > 0);

}

template<typename T>
vector<DataPoint<int>> FilterDataset<T>::getNearestNeighbors(const Query<T> &q, const int &k){
     // vector<pair<int,float>> distances;
     // for(int i = 0; i < datapoints[q.v][i].vec.size(); i++) {
     //     distances.push_back({ i , euclideanDistance(q.vec,datapoints[q.v][i].vec)});
     // }
     //
     // auto comparator = [&](const pair<int, float>& a, const pair<int, float>& b) {
     //     return a.second < b.second; // Sort by distance in ascending order
     // };
     //
     // sort(distances.begin(), distances.end(), comparator);
     //
     // vector<int> neighbors;
     // for (int i = 0; i < k && i < distances.size(); i++) {
     //     neighbors.push_back(distances[i].first); // Push only the index (first element of pair)
     // }
     //
     // return neighbors;

    return vector<DataPoint<int>>();
}


template <typename T>
void FilterDataset<T>::print() {

    cout << "d = " << FILTER_DATASET_DIMENSION - 2 << endl;
    cout << "numOfDataPoints = " << numOfDataPoints << endl;
    cout << "sizeof(T) = " << sizeof(T) << endl;

    for(int i = 0; i < numOfDataPoints; i++) {
        fprintf(stdout, "\e[36mvector[ %d ] is:   \e[0m\n",i);
    }
}

// explicit template instantiations
template class FilterDataset<float>;
template class FilterDataset<int>;
template class FilterDataset<char>;