//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"

template<typename T>
DataSet<T>::DataSet() {}

template <typename T>
DataSet<T>::DataSet(const string& filename) {
    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);

    // Read the vector size (dimension)
    int globalDim;
    file.read(reinterpret_cast<char*>(&globalDim), sizeof(int));
    const int vectorSize = sizeof(int) + globalDim * sizeof(T);

    // Find total number of vectors in the file
    file.seekg(0, ios::end);
    int n = file.tellg() / vectorSize;
    file.seekg(0, ios::beg);

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

    this->vectors = vectors;
    assert(!vectors.empty());
    numOfVectors = vectors.size();
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