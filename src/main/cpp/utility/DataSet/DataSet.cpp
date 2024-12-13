//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"

#include "../DataPoint/DataPoint.h"

template<typename T>
DataSet<T>::DataSet() {}

template <typename T>
DataSet<T>::DataSet(const string& filename) {
    const string filePath(RESOURCES_P + filename);

    ifstream file(filePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);

    // Read the vector size (dimension)
    int globalDim;
    file.read(reinterpret_cast<char*>(&globalDim), sizeof(int));
    const int vectorSize = sizeof(int) + globalDim * sizeof(T);

    // Find total number of vectors in the file
    file.seekg(0, ios::end);
    const int n = file.tellg() / vectorSize;
    file.seekg(0, ios::beg);

    // Read the vectors
    vector<DataPoint<T>> datapoints;

    for (int i = 0; i < n; ++i) {
        DataPoint<T> dataPoint = readDataPoint(file,i,globalDim);
        datapoints.push_back(dataPoint);
    }

    file.close();

    d = globalDim;

    this->datapoints = datapoints;
    assert(!datapoints.empty());
    numOfDatapoints = datapoints.size();
}

template<typename T>
DataPoint<T> DataSet<T>::readDataPoint(ifstream& file,int id,int& globalDim) {

    int dim;
    file.read(reinterpret_cast<char*>(&dim), sizeof(int));

    T data;
    vector<T> vec(globalDim);

    for(int j = 0; j < globalDim; j++) {
        file.read(reinterpret_cast<char*>(&data),sizeof(T));
        vec[j] = data;
    }

    return DataPoint<T>(id,-1,-1,vec);
}

template <typename T>
void DataSet<T>::vecsWrite(const string& filename, const vector<vector<T>>& vectors,int globalDim) {
    const string dataFilePath(RESOURCES_P + filename);

    ofstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename + " for writing.");

    // Read the vector size (dimension)
    file.write(reinterpret_cast<char*>(&globalDim), sizeof(int));

    constexpr int placeholder = -1;

    // Write each vector's dimension and data to the file
    for (const auto& vec : vectors) {
        int dimension = vec.size();

        // Write the dimension
        if (!file.write(reinterpret_cast<const char*>(&dimension), sizeof(int)))
            throw runtime_error("I/O error: Unable to write vector dimension to the file.");


        // Write all elements of the vector
        for (int j = 0; j < dimension; j++){
            if (!file.write(reinterpret_cast<const char*>(&vec[j]), sizeof(T)))
                throw runtime_error("I/O error: Unable to write vector data to the file.");
        }
        for (int j = dimension; j < globalDim; j++) {
            if (!file.write(reinterpret_cast<const char*>(&placeholder), sizeof(T)))
                throw runtime_error("I/O error: Unable to write vector data to the file.");
        }

    }

    file.close();
}

template <typename T>
void DataSet<T>::print() {

    cout << "d=" << d << endl;
    cout << "numOfVectors=" << numOfDatapoints << endl;
    cout << "sizeof(T)=" << sizeof(T) << endl;

    for(int i = 0; i < numOfDatapoints; i++) {
        Utils<T>::printVec(datapoints[i].vec);
        DIVIDER
    }
}



// explicit template instantiations
template class DataSet<float>;
template class DataSet<int>;
template class DataSet<char>;