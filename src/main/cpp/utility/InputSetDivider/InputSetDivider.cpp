//
// Created by mkorres on 12/3/2024.
//

#include "InputSetDivider.h"

template<typename T>
void InputSetDivider<T>::divideFilterDataset(const string& filename) {

    string dataFilePath(RESOURCES_P + filename);

    ifstream file(dataFilePath, ios::binary);
    if (!file.is_open()) throw runtime_error("I/O error: Unable to open the file " + filename);

    uint32_t numOfDataPoints;
    file.read(reinterpret_cast<char*>(&numOfDataPoints), sizeof(int));

    T data;
    DataPoint<T> dataPoint;
    vector<DataPoint<T>> dataPointsVec = vector<DataPoint<T>>();
    int subsetIndex = 0;
    for (int i = 1; i <= numOfDataPoints; ++i) {
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

        dataPointsVec.push_back(dataPoint);

        if(i % 100000 ==  0) {
            
            size_t pos = dataFilePath.find_last_of(".bin");
            string outputFilename;
            if (pos != string::npos) {
                outputFilename = dataFilePath.substr(0, pos - 3) + "/subset" + to_string(subsetIndex) + ".bin";
            }
            writeFilterDatasetSubset(dataPointsVec,outputFilename);
            dataPointsVec.clear();
            subsetIndex++;
        }

    }

    file.close();

}

template <typename T>
void writeFilterDatasetSubset(vector<DataPoint<T>> dataPointsVec,const string& filename) {
    const filesystem::path filePath(filename);
    filesystem::create_directories(filePath.parent_path());

    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("I/O error: Unable to open the file for writing: " + filename);
    }

    uint32_t numOfDataPoints = dataPointsVec.size();

    file.write(reinterpret_cast<const char*>(&numOfDataPoints), sizeof(uint32_t));

    // Write the DataPoints
    for (const auto& point : dataPointsVec) {
        // Write C
        float C = static_cast<float>(point.C);
        file.write(reinterpret_cast<const char*>(&C), sizeof(float));

        // Write timestamp (T)
        file.write(reinterpret_cast<const char*>(&point.T), sizeof(float));

        // Write vector data
        for (const auto& val : point.vec) {
            file.write(reinterpret_cast<const char*>(&val), sizeof(T));
        }
    }

    file.close();
}

// explicit template instantiations
template class InputSetDivider<float>;
template class InputSetDivider<int>;
template class InputSetDivider<char>;