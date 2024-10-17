# include <iostream>
# include <cstring>
# include "utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
#include "utility/Graph/Graph.h"
# include "utility/Utils/Utils.h"
# include "utility/VectorFileType/VectorFileType.h"

using namespace std;

void initializeDatasets(DataSet<float>& baseDataSet, DataSet<float>& queryDataSet, DataSet<int>& groundtruthDataSet,char* argv[],int argc) ;

int main(int argc,char* argv[]) {
	// if(argc < 9)exit(EXIT_FAILURE

	vector<vector<int>> vecs {
		vector<int> {0},
		vector<int> {1},
		vector<int> {2},
		vector<int> {3},
		vector<int> {100},
		vector<int> {101},
		vector<int> {102},
		vector<int> {103},
		vector<int> {200},
		vector<int> {201},
		vector<int> {202},
		vector<int> {203},
	};

	int k = 2;

	Graph graph(vecs,k);

	// graph.printGraph();

	vector<int> q {204};
	vector<Edge> qNeighbors = graph.getNearestNeighbors(q);

	graph.printVectorNeighbors(qNeighbors);

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