//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"

template<typename T>
DataSet<T>::DataSet() {}

template <typename T>
DataSet<T>::DataSet(const string& dataFileName) {
    this->vectors = this->vecsRead(dataFileName,{1,-1});
    assert(!vectors.empty());
    this->d = this->vectors[0].size();
    this->numOfVectors = this->vectors.size();
}

template <typename T>
vector<vector<T>> DataSet<T>::vecsRead(const string& filename, pair<int, int> bounds) {

    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file " + filename);
    }

    // Read the vector size (dimension)
    int d;
    file.read(reinterpret_cast<char*>(&d), sizeof(int));
    int vectorSize = sizeof(int) + d * sizeof(T);

    // Find total number of vectors in the file
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    int bmax = fileSize / vectorSize;

    int a = bounds.first;
    int b = (bounds.second == -1) ? bmax : bounds.second;

    // Ensure the bounds are valid
    assert(a >= 1);
    if (b > bmax) b = bmax;
    if (b == 0 || b < a) return {};

    // Calculate the number of vectors to read
    int n = b - a + 1;

    // Seek to the starting position
    file.seekg((a - 1) * vectorSize, ios::beg);

    // Read the vectors
    vector<vector<T>> vectors(n, vector<T>(d));

    T data;

    for (int i = 0; i < n; ++i) {
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        assert(dim == d);

        for(int j = 0; j < d; j++) {
            file.read(reinterpret_cast<char*>(&data),sizeof(T));
            vectors[i][j] = data;
        }

    }

    file.close();
    return vectors;
}

template <typename T>
void DataSet<T>::vecsWrite(const string& filename, const vector<vector<T>>& vectors) {
    const string dataFilePath(RESOURCES_P + filename);

    ofstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename + " for writing.");

    // Iterate over each vector and write its dimension and data to the file
    for (const auto& vec : vectors) {
        int dimension = vec.size();
        file.write(reinterpret_cast<const char*>(&dimension), sizeof(int));  // Write dimension

        // Write all elements of the vector
        for (const T& value : vec) {
            file.write(reinterpret_cast<const char*>(&value), sizeof(T));
        }
    }

    file.close();
}

template <typename T>
int DataSet<T>::getD() {
    return this->d;
}

template <typename T>
int DataSet<T>::getNumOfVectors() {
    return this->numOfVectors;
}

template <typename T>
vector<vector<T>> DataSet<T>::getVectors() {
    return this->vectors;
}

template<typename T>
vector<T> DataSet<T>::getVector(int id) {
    return this->vectors[id];
}

template <typename T>
void DataSet<T>::print() {

    cout << "d=" << d << endl;
    cout << "numOfVectors=" << numOfVectors << endl;
    cout << "sizeof(T)=" << sizeof(T) << endl;

    for(int i = 0; i < numOfVectors; i++) {
        Utils<T>::printVec(vectors[i]);
    }
}



// explicit template instantiations
template class DataSet<float>;
template class DataSet<int>;
template class DataSet<char>;