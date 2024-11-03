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
	int k = 100; // groundtruthDataSet.getD();
	int L = 250;
	int R = 60;
	double a = 1.2;


	Graph graph(baseDataSet.getVectors(),L,R,k,a);

	clock_t start = clock();

	graph.vamana();

	clock_t end = clock();
	double timeSpent = (double)(end - start) / CLOCKS_PER_SEC;
	printf("index build: %f seconds\n", timeSpent);

	vector<Edge> nearestNeighborsEdges;
	vector<int> groundTruthNearestNeighbors;

	int misses = 0;

	int medoidId = graph.medoid();

	vector<float> q;
	for(int i = 0; i < numOfQueries;i++) {
		q = queryDataSet.getVector(i);
		const auto& [neighbors,v] = graph.greedySearch(medoidId,q,k,L);

		groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
		double vecsDiff = Graph<int>::equals(neighbors,groundTruthNearestNeighbors);


		cout << "Q" << i << ": " << vecsDiff * 100 << "%" << endl;

	}


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

	baseDataSet = DataSet<float>(baseVectorsDataFileName);
	queryDataSet = DataSet<float>(queryVectorsDataFileName);
	groundtruthDataSet = DataSet<int>(groundtruthVectorsDataFileName);

}