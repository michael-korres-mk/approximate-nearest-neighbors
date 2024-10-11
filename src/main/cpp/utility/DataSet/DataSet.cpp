//
// Created by mkorres on 10/9/2024.
//

# include "DataSet.h"
# include "../Utils/Utils.h"

template<typename T>
DataSet<T>::DataSet() {
}

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
    for(int j = 0; j < d; j++) {
        dist += (v2[j] - v1[j]) * (v2[j] - v1[j]);
    }

    return sqrt(dist);
}

template<typename T>
bool DataSet<T>::equals(vector<T>& v1, vector<T>& v2) {
    const int dim = v1.size();
    for(int i = 0; i < dim; i++){
        if(v1[i] != v2[i]) {
            if(( i == dim -1) || (v2[i+1] != v2[i+1])) { // lookahead for equally distant neighbors handling
                return false;
            }
            else {
                i++; // do not check next
            }
        }

    }

    return true;
}


template<typename T>
int DataSet<T>::getNearestNeighbor(const vector<T>& q) {
    int nnId = 0;
    float minDistance = numeric_limits<float>::max();
    float currDist;


    for(int i = 0; i < this->getNumOfVectors(); i++) {
        currDist = euclideanDistance(q,this->vectors[i]);
        if(currDist < minDistance) {
            minDistance = currDist;
            nnId = i;
        }
    }

    return nnId;
}

template<typename T>
vector<int> DataSet<T>::getNearestNeighbors(const vector<T>& q,const int& k) {
    vector<pair<int,float>> distances;
    for(int i = 0; i < this->getNumOfVectors(); i++) {
        distances.push_back({ i , euclideanDistance(q,this->vectors[i])});
    }

    auto comparator = [&](const pair<int, float>& a, const pair<int, float>& b) {
        return a.second < b.second; // Sort by distance in ascending order
    };

    sort(distances.begin(), distances.end(), comparator);

    vector<int> neighbors;
    for (int i = 0; i < k && i < distances.size(); i++) {
        neighbors.push_back(distances[i].first); // Push only the index (first element of pair)
    }

    return neighbors;
}

template <typename T>
void DataSet<T>::printVector(vector<T> v) {
    for(int j = 0; j < v.size() - 1; j++) {
        cout << v[j] << " ";
    }
    cout << v[v.size() - 1] << endl;
}



// explicit template instantiations
template class DataSet<float>;
template class DataSet<int>;
template class DataSet<char>;