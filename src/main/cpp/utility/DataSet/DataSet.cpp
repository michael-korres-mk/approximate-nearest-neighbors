//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"
#include "../Utils/Utils.h"

template <typename T>
DataSet<T>::DataSet(char* dataFileName) {
    // open file located at src/main/resources/
    char dataFilePath[BUFFER_SIZE] = {'\0'};
    strcat(dataFilePath,SOURCE_D);strcat(dataFilePath,MAIN_D);strcat(dataFilePath,RESOURCES_D);strcat(dataFilePath,dataFileName);
    int fd = open(dataFilePath,O_RDONLY); // Open data file
    if (fd == -1) {perror("open");exit(EXIT_FAILURE);}

    // get vector dimension
    int vectorDimension;
    read(fd, &vectorDimension, sizeof(int));

    // get vector size
    int vectorSize = 4 + vectorDimension * 4;
    int numOfVectors = (int) (lseek(fd, 0, SEEK_END)/ vectorSize);

    // reset file pointer
    lseek(fd, 0, SEEK_SET);

    this->d = vectorDimension;
    this->numOfVectors = numOfVectors;

    // read vectors
    T** vectors = new T*[numOfVectors];
    int vec_d;
    for(int i = 0; i < numOfVectors; i++) {
        vectors[i] = new T[vectorSize];
        read(fd, &vec_d, sizeof(int));
        assert(vec_d == this->getD());
        for(int j = 0; j < this->getD(); j++) {
            read(fd, &vectors[i][j], sizeof(T));
        }
    }

    close(fd);

    this->vectors = vectors;

}

template <typename T>
DataSet<T>::~DataSet() {
    for(int i = 0; i < this->numOfVectors; i++) {
        delete[] vectors[i];
    }
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
T** DataSet<T>::getVectors() {
    return this->vectors;
}

template <typename T>
void DataSet<T>::print() {

    cout << "d=" << d << endl;
    cout << "numOfVectors=" << numOfVectors << endl;
    cout << "sizeof(T)=" << sizeof(T) << endl;

    T** vectors = this->getVectors();

    for(int i = 0; i < this->getNumOfVectors(); i++) {
        fprintf(stdout, "\e[36mvector[ %d ] is:   \e[0m\n",i);
        for(int j = 0; j < this->getD() - 1; j++) {
            cout << vectors[i][j] << " ";
        }
        cout << setw(4) << fixed << setprecision(4) << vectors[i][this->getD() - 1] << endl;
    }
}

// explicit template instantiations
template class DataSet<float>;
template class DataSet<int>;
template class DataSet<char>;