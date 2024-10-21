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
	int k = groundtruthDataSet.getD();
	int R = 150;
	double a = 1.8;

	// Graph graph(vecs,L,R,a);

	vector<vector<float>> subset;
	for(int i = 0; i < 2000; i++)subset.push_back(baseDataSet.getVector(i));

	Graph graph(baseDataSet.getVectors(),R,k,a);

	vector<Edge> nearestNeighborsEdges;
	vector<int> groundTruthNearestNeighbors;

	int misses = 0;

	int medoidId = graph.medoid();

	vector<float> q;
	for(int i = 0; i < numOfQueries;i++) {
		q = queryDataSet.getVector(i);
		pair<vector<int>,vector<int>> gS = graph.greedySearch(medoidId,q,k);

		vector<int> neighbors = gS.first;
		vector<int> v = gS.second;

		groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
		bool equalVecs = Graph<int>::equals(neighbors,groundTruthNearestNeighbors);

		if(equalVecs) {
			cout << "Q" << i << ": " << "SUCCESS" << endl;
		}
		else {
			cout << "Q" << i << ": " << "FAILURE" << endl;
			misses++;
		}
	}

	if(misses > 0) cout << "misses: " << misses << endl;


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