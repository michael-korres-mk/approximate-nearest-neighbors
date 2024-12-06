//
// Created by mkorres on 12/5/2024.
//

# include <chrono>
# include <iostream>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
# include "../filterann/FilterQuerySet/FilterQuerySet.h"
# include "../filterann/FilterDataset/FilterDataset.h"
# include "../utility/Utils/Utils.h"
# include "../utility/DataSet/DataSet.h"
#include "../utility/VamanaContainer/VamanaContainer.h"

using namespace std;

int k = 100;
int L;
int R;
double a;
char* dataFilename;
char* queriesFilename;
char* groundtruthFilename;

void initializeDatasets(FilterDataset<float>& fdataset, FilterQuerySet<float>& querySet,char* argv[],int argc) {

	for(int i = 1; i < argc;i++){	// Get arguments
		if (strcmp(argv[i],"-bv") == 0) {
			dataFilename = argv[i+1];
		} else if (strcmp(argv[i],"-qv") == 0) {
			queriesFilename = argv[i+1];
		}
		else if (strcmp(argv[i],"-gv") == 0) {
			groundtruthFilename = argv[i+1];
		}
	}

	cout << "Base Dataset: " << dataFilename << endl;
	cout << "Query Dataset: " << queriesFilename << endl;

	fdataset = FilterDataset<float>(dataFilename);
	querySet = FilterQuerySet<float>(queriesFilename);

}

// Function to calculate Euclidean distance between two vectors
template<typename T>
double euclideanDistance(const vector<T>& a, const vector<T>& b) {
	double sum = 0.0;
	for (size_t i = 0; i < a.size(); i++) {
		sum += (a[i] - b[i]) * (a[i] - b[i]);
	}
	return sqrt(sum);
}

template<typename T>
vector<int> filteredNN(FilterDataset<T>& fdataset,const vector<T>& queryVec,int& k,int filterValue) {
	VamanaContainer nearestNeighbors(2*k);

	for (int i = 0; i < fdataset.numOfDataPoints; ++i) {
		auto datapoint = fdataset.datapoints[i];
		if (filterValue != -1 && filterValue != datapoint.C) continue;
		double distance = euclideanDistance(fdataset.datapoints[i].vec, queryVec);
		nearestNeighbors.insert({i, distance});
	}

	return nearestNeighbors.subset(k);
}


template <typename T>
vector<vector<int>> runQueries(FilterDataset<T>& fdataset,FilterQuerySet<T>& qset) {
	int nq = qset.numOfQueries;
	vector<vector<int>> vectors(nq);

	for(int i = 0; i < nq; i++) {
		const int qtype = qset.queries[i].queryType;
		int v = qset.queries[i].v;
		vector<T> queryVec = qset.queries[i].vec;

		if(qtype == 0) {  // only ANN
			vectors[i] = filteredNN(fdataset,queryVec,k,-1);
		}
		else if(qtype == 1) { // equal + ANN
			vectors[i] = filteredNN(fdataset,queryVec,k,v);
		}

	}

	return vectors;
}

int main(int argc,char* argv[]) {

	FilterDataset<float> dataset;
	FilterQuerySet<float> querySet;

	initializeDatasets(dataset,querySet,argv,argc);

	const auto vectors = runQueries<float>(dataset,querySet);

	DataSet<int>::vecsWrite(groundtruthFilename,vectors,k);

	auto dset = DataSet<int>("dummy-groundtruth.bin");

	dset.print();

}
