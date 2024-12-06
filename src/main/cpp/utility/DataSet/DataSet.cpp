//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"

template<typename T>
DataSet<T>::DataSet() {}

template <typename T>
DataSet<T>::DataSet(const string& dataFileName) {
    vectors = vecsRead(dataFileName,{1,-1});
    assert(!vectors.empty());
    numOfVectors = vectors.size();
}

template <typename T>
vector<vector<T>> DataSet<T>::vecsRead(const string& filename, pair<int, int> bounds) {

    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);

    // Read the vector size (dimension)
    int globalDim;
    file.read(reinterpret_cast<char*>(&globalDim), sizeof(int));
    int vectorSize = sizeof(int) + globalDim * sizeof(T);

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
    vector<vector<T>> vectors(n, vector<T>(globalDim));

    T data;

    for (int i = 0; i < n; ++i) {

        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));

        for(int j = 0; j < globalDim; j++) {
            file.read(reinterpret_cast<char*>(&data),sizeof(T));
            vectors[i][j] = data;
        }

    }

    file.close();

    d = globalDim;

    return vectors;
}

template <typename T>
void DataSet<T>::vecsWrite(const string& filename, const vector<vector<T>>& vectors,int globalDim) {
    const string dataFilePath(RESOURCES_P + filename);

    ofstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename + " for writing.");


    // Read the vector size (dimension)
    file.write(reinterpret_cast<char*>(&globalDim), sizeof(int));

    int placeholder = -1;

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
int DataSet<T>::getD() {
    return d;
}

template <typename T>
int DataSet<T>::getNumOfVectors() {
    return numOfVectors;
}

template <typename T>
vector<vector<T>> DataSet<T>::getVectors() {
    return vectors;
}

template<typename T>
vector<T> DataSet<T>::getVector(int id) {
    return vectors[id];
}

template <typename T>
void DataSet<T>::print() {

    cout << "d=" << d << endl;
    cout << "numOfVectors=" << numOfVectors << endl;
    cout << "sizeof(T)=" << sizeof(T) << endl;

    for(int i = 0; i < numOfVectors; i++) {
        Utils<T>::printVec(vectors[i]);
        DIVIDER
    }
}



// explicit template instantiations
template class DataSet<float>;
template class DataSet<int>;
template class DataSet<char>;