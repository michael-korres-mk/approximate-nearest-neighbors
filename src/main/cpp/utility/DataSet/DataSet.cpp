//
// Created by mkorres on 10/9/2024.
//

#include "DataSet.h"
#include "../Utils/Utils.h"

char srcD[] = "src/";
char mainD[] = "main/";
char resourcesD[] = "resources/";


DataSet::DataSet(char* dataFileName) {
    char dataFilePath[BUFFER_SIZE] = {'\0' };

    strcat(dataFilePath,srcD);strcat(dataFilePath,mainD);strcat(dataFilePath,resourcesD);strcat(dataFilePath,dataFileName);

    int baseFd = open(dataFilePath,O_RDONLY); // Open data file

    if (baseFd == -1) {perror("open");exit(EXIT_FAILURE);}

    int d;
    read(baseFd, &d, sizeof(int));

    cout << "d=" << d << endl;

    int vectorSize = 4 + d * 4;

    cout << "vectorSize=" << vectorSize << endl;

    int numOfVectors = (int) (lseek(baseFd, 0, SEEK_END)/ vectorSize);    // Get number of Vectors

    cout << "numOfVectors=" << numOfVectors << endl;

    lseek(baseFd, sizeof(int), SEEK_SET);

    float** vectors = new float*[numOfVectors];

    for(int i = 0; i < numOfVectors; i++) {
        vectors[i] = new float[vectorSize];
        for(int j = 0; j < vectorSize; j++) {
            read(baseFd, &vectors[i][j], sizeof(float));
        }
    }

    close(baseFd);

    this->d = d;
    this->numOfVectors = numOfVectors;
    this->vectors = vectors;

}

int DataSet::getD() {
    return this->d;
}

int DataSet::getNumOfVectors() {
    return this->numOfVectors;
}

float** DataSet::getVectors() {
    return this->vectors;
}

void DataSet::print() {
    float ** vectors = this->getVectors();

    for(int i = 0; i < this->getNumOfVectors(); i++) {
        fprintf(stdout, "\e[36mvector[ %d ] is:   \e[0m\n",i);
        for(int j = 0; j < this->getD() - 1; j++) {
            cout << vectors[i][j] << " ";
        }
        cout << setw(4) << fixed << setprecision(4) << vectors[i][this->getD() - 1] << endl;
        cout << setw(4) << fixed << setprecision(4) << vectors[i][0] << endl;

    }
}
