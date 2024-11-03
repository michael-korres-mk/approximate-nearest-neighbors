#include <chrono>
# include <iostream>
# include <cstring>
# include "utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
#include "utility/Graph/Graph.h"
# include "utility/Utils/Utils.h"

using namespace std;

void initializeDatasets(DataSet<float>& baseDataSet, DataSet<float>& queryDataSet, DataSet<int>& groundtruthDataSet,char* argv[],int argc) ;

int main(int argc,char* argv[]) {
	// if(argc < 9)exit(EXIT_FAILURE

	DataSet<float> baseDataSet;
	DataSet<float> queryDataSet;
	DataSet<int> groundtruthDataSet;

	initializeDatasets(baseDataSet,queryDataSet,groundtruthDataSet,argv,argc);


	int numOfQueries = queryDataSet.getNumOfVectors();

	int k; // groundtruthDataSet.getD();
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

	Graph graph(baseDataSet.getVectors(),L,R,k,a);

	auto start = chrono::high_resolution_clock::now();

	graph.vamana();

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	cout << "index build: " << duration << " ms" << endl;

	vector<Edge> nearestNeighborsEdges;
	vector<int> groundTruthNearestNeighbors;

	int medoidId = graph.medoid();

	vector<float> q;
	double totalKRecall = 0.0;
	for(int i = 0; i < numOfQueries;i++) {
		q = queryDataSet.getVector(i);
		const auto& [neighbors,v] = graph.greedySearch(medoidId,q,k,L);

		groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
		double kRecall = Graph<int>::equals(neighbors,groundTruthNearestNeighbors);
		totalKRecall += kRecall;
		// cout << "Q" << i << ": " << kRecall * 100 << "%" << endl;
	}

	printf("k-recall@k: %.2lf%%\n",(totalKRecall / numOfQueries) * 100);

}

void initializeDatasets(DataSet<float>& baseDataSet, DataSet<float>& queryDataSet, DataSet<int>& groundtruthDataSet,char* argv[],int argc)  {

	string baseVectorsDataFileName;
	string queryVectorsDataFileName;
	string groundtruthVectorsDataFileName;

	for(int i = 1; i < argc;i++){	// Get arguments
		if (strcmp(argv[i],"-bv") == 0) {
			baseVectorsDataFileName = argv[i+1];
		} else if (strcmp(argv[i],"-qv") == 0) {
			queryVectorsDataFileName = argv[i+1];
		} else if (strcmp(argv[i],"-gv") == 0) {
			groundtruthVectorsDataFileName = argv[i+1];
		}
	}

	cout << "Base Dataset: " << baseVectorsDataFileName << endl;
	cout << "Query Dataset: " << queryVectorsDataFileName << endl;
	cout << "Ground-truth Dataset: " << groundtruthVectorsDataFileName << endl;

	cout << "-----------------------------------------------------------" << endl;

	auto start = chrono::high_resolution_clock::now();
	baseDataSet = DataSet<float>(baseVectorsDataFileName);

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	cout << "base dataset load: " << duration << " ms" << endl;

	queryDataSet = DataSet<float>(queryVectorsDataFileName);
	groundtruthDataSet = DataSet<int>(groundtruthVectorsDataFileName);

	cout << "-----------------------------------------------------------" << endl;

}