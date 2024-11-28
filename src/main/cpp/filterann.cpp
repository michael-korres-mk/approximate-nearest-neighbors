//
// Created by mkorres on 11/10/2024.
//

# include <chrono>
# include <iostream>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
# include "utility/FilterGraph/FilterGraph.h"
# include "utility/Utils/Utils.h"
# include "utility/FilterDataset/FilterDataset.h"
# include "utility/FilterQuerySet/FilterQuerySet.h"

using namespace std;

void initializeDatasets(FilterDataset<float>& dataset, FilterQuerySet<float>& querySet,char* argv[],int argc) ;

int main(int argc,char* argv[]) {
	Utils<char>::printDivider();

	FilterDataset<float> dataset;
	FilterQuerySet<float> querySet;

	initializeDatasets(dataset,querySet,argv,argc);

	Utils<char>::printDivider();

	// int numOfQueries = querySet.getNumOfQueries();

	int k;
	int L;
	int R;
	double a;

	for(int i = 1; i < argc;i++){	// Get arguments
		if (strcmp(argv[i],"-k") == 0) {
			k = atoi(argv[i+1]);
		} else if (strcmp(argv[i],"-L") == 0) {
			L = atoi(argv[i+1]);
		} else if (strcmp(argv[i],"-R") == 0) {
			R = atoi(argv[i+1]);
		} else if (strcmp(argv[i],"-a") == 0) {
			a = atof(argv[i+1]);
		}
	}


	cout << "k = " << k << endl;
	cout << "L = " << L << endl;
	cout << "R = " << R << endl;
	cout << "a = " << a << endl;

	Utils<char>::printDivider();

    FilterGraph<float> graph(dataset.dataPoints,L,R,k,a, 10);

    cout << "Start findMedoid()" << endl;
    map<int,int> m = graph.findMedoid();
    cout << "Finish findMedoid()" << endl;

    graph.stitchedVamana();

}

void initializeDatasets(FilterDataset<float>& dataset, FilterQuerySet<float>& querySet,char* argv[],int argc) {
	string baseVectorsDataFileName;
	string queryVectorsDataFileName;

	for(int i = 1; i < argc;i++){	// Get arguments
		if (strcmp(argv[i],"-bv") == 0) {
			baseVectorsDataFileName = argv[i+1];
		} else if (strcmp(argv[i],"-qv") == 0) {
			queryVectorsDataFileName = argv[i+1];
		}
	}

	cout << "Base Dataset: " << baseVectorsDataFileName << endl;
	cout << "Query Dataset: " << queryVectorsDataFileName << endl;

	Utils<char>::printDivider();

	auto datasetStart = chrono::high_resolution_clock::now();
	dataset = FilterDataset<float>(baseVectorsDataFileName);

	auto datasetEnd = chrono::high_resolution_clock::now();
	auto datasetDuration = chrono::duration_cast<chrono::milliseconds>(datasetEnd - datasetStart).count();
	cout << "base dataset load: " << datasetDuration << " ms" << endl;

	Utils<char>::printDivider();

	 auto queryDatasetStart = chrono::high_resolution_clock::now();
	 querySet = FilterQuerySet<float>(queryVectorsDataFileName);

	 auto queryDatasetEnd = chrono::high_resolution_clock::now();
	 auto queryDatasetDuration = chrono::duration_cast<chrono::milliseconds>(queryDatasetEnd - queryDatasetStart).count();
	 cout << "query dataset load: " << queryDatasetDuration << " ms" << endl;

}