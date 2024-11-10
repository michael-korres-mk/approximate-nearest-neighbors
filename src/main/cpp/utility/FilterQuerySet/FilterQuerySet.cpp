//
// Created by mkorres on 11/11/2024.
//

#include "FilterQuerySet.h"

#include "../Utils/Utils.h"
#include "FilterQuerySet.h"

#include <cstdint>

template<typename T>
FilterQuerySet<T>::FilterQuerySet() {}

template<typename T>
FilterQuerySet<T>::FilterQuerySet(const string &dataFileName) {
    queries = vecsRead(dataFileName);
    assert(!queries.empty());
    numOfQueries = queries.size();
}

template<typename T>
vector<Query<T>> FilterQuerySet<T>::vecsRead(const string &filename) {

    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);


    // Read the vector size (dimension)
    uint32_t numOfQueries;
    file.read(reinterpret_cast<char*>(&numOfQueries), sizeof(int));

    // Read the vectors
    vector<Query<T>> queries = vector<Query<T>>(numOfQueries);
    T data;

    for (int i = 0; i < numOfQueries; ++i) {
        queries[i] = Query<T>(-1,-1,-1,-1,vector<T>(FILTER_QUERY_SET_DIMENSION - 4));

        float queryType;
        file.read(reinterpret_cast<char*>(&queryType), sizeof(float));

        float v;
        file.read(reinterpret_cast<char*>(&v), sizeof(float));

        float l;
        file.read(reinterpret_cast<char*>(&l), sizeof(float));

        float r;
        file.read(reinterpret_cast<char*>(&r), sizeof(float));


        queries[i].queryType = static_cast<int>(queryType);
        queries[i].v = static_cast<int>(v);
        queries[i].l = l;
        queries[i].r = r;


        for(int j = 0; j < FILTER_QUERY_SET_DIMENSION - 4; j++) {
            file.read(reinterpret_cast<char*>(&data),sizeof(T));
            queries[i].vec[j] = data;
        }

    }

    file.close();
    return queries;

}


template <typename T>
void FilterQuerySet<T>::print() {

    cout << "d = " << FILTER_QUERY_SET_DIMENSION - 4 << endl;
    cout << "numOfQueries = " << numOfQueries << endl;
    cout << "sizeof(T) = " << sizeof(T) << endl;

    for(int i = 0; i < numOfQueries; i++) {
        fprintf(stdout, "\e[36mvector[ %d ] is:   \e[0m\n",i);
        queries[i].print();
    }
}

// explicit template instantiations
template class FilterQuerySet<float>;
template class FilterQuerySet<int>;
template class FilterQuerySet<char>;