//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"
#include "../Utils/Utils.h"

template <typename T>
DataSet<T>::DataSet(char* dataFileName) {
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

    // cout << "d: " << d << endl;
    // cout << "vec_sizeof: " << vec_sizeof << endl;
    // cout << "bmax: " << bmax << endl;

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

    for (int i = 0; i < n; ++i) {
        int dim;
        file.read(reinterpret_cast<char*>(&dim), sizeof(int)); // Read the dimension (should be d)

        // Read the vector components
        file.read(reinterpret_cast<char*>(vectors[i].data()), d * sizeof(T));

        // Check if the dimension read matches the expected dimension
        assert(dim == d);
    }

    file.close();
    return vectors;
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

template <typename T>
void DataSet<T>::print() {

    cout << "d=" << d << endl;
    cout << "numOfVectors=" << numOfVectors << endl;
    cout << "sizeof(T)=" << sizeof(T) << endl;

    for(int i = 0; i < this->getNumOfVectors(); i++) {
        fprintf(stdout, "\e[36mvector[ %d ] is:   \e[0m\n",i);
        for(int j = 0; j < this->getD() - 1; j++) {
            cout << vectors[i][j] << " ";
        }
        cout << setw(4) << fixed << setprecision(4) << vectors[i][this->getD() - 1] << endl;
    }
}

template<typename T>
float DataSet<T>::euclideanDistance(vector<T> v1, vector<T> v2) {
    int d = v1.size();
    float dist = 0;
    for(int i = 0; i < d; i++) {
        dist += pow(v1[i] - v2[i], 2);
    }

    return sqrt(dist);
}

// explicit template instantiations
template class DataSet<float>;
template class DataSet<int>;
template class DataSet<char>;