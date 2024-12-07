#include <chrono>
# include <iostream>
# include <cstring>
#include "../utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
# include <filesystem>
#include "Graph/Graph.h"
#include "../utility/Utils/Utils.h"

using namespace std;

string dataFilename;
string queriesFileName;
string groundtruthFileName;

int k;
int L;
int R;
double a;


void initializeDatasets(DataSet<float>& dataset, DataSet<float>& querySet, DataSet<int>& groundtruthSet)  {

	cout << "Base Dataset: " << dataFilename << endl;
	cout << "Query Dataset: " << queriesFileName << endl;
	cout << "Ground-truth Dataset: " << groundtruthFileName << endl;

	DIVIDER

	TIMER_BLOCK("Base dataset load",
		dataset = DataSet<float>(dataFilename);
	)

	cout << "Num of datapoints: " << dataset.getNumOfVectors() << endl;

	DIVIDER

		TIMER_BLOCK("Query dataset load",
			querySet = DataSet<float>(queriesFileName);
		)

		cout << "Num of queries: " << querySet.getNumOfVectors() << endl;

	DIVIDER

	TIMER_BLOCK("Ground-truth dataset load",
		groundtruthSet = DataSet<int>(groundtruthFileName);
	)
}

template <typename T>
void runQueries(Graph<T> graph,DataSet<T> qset,DataSet<int>& groundtruthDataSet) {
	DIVIDER


	int medoidId = graph.medoid();
	vector<float> q;
	double totalKRecall = 0.0;
	const int nq = qset.getNumOfVectors();

	if(nq == 0) throw runtime_error("No queries given ...");

	printf("running queries ... \n");
	for(int i = 0; i < nq; i++) {

		if (nq > 10 && i > 0 && i % (nq / 10) == 0) printf("queries completed %d %% ... \n", i * 100 / nq);

		q = qset.getVector(i);

		const auto& [neighbors,v] = graph.greedySearch(medoidId,q,k,L);
		vector<int> groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);

		const double kRecall = Graph<int>::equals(neighbors,groundTruthNearestNeighbors);
		totalKRecall += kRecall;

	}

	printf("queries completed 100 %% ... \n");

	DIVIDER

	printf("k-recall@k: %.2lf%%\n",(totalKRecall / nq) * 100);

}

int main(int argc,char* argv[]) {
	DIVIDER

	for (int i = 1; i < argc; i++) {    // Get arguments
		GET_INT_ARG("-k", k)
		GET_INT_ARG("-L", L)
		GET_INT_ARG("-R", R)
		GET_DOUBLE_ARG("-a", a)
		GET_STRING_ARG("-bv", dataFilename)
		GET_STRING_ARG("-qv", queriesFileName)
		GET_STRING_ARG("-gv", groundtruthFileName)
	}


	PRINT_VAR(k)
	PRINT_VAR(L)
	PRINT_VAR(R)
	PRINT_VAR(a)

	DIVIDER

	DataSet<float> dataset,querySet;
	DataSet<int> groundtruthSet;

	initializeDatasets(dataset,querySet,groundtruthSet);

	Graph<float> graph;

	DIVIDER

	const string vamanaFilename = "vamana_graph.bin";

	if(filesystem::path filePath(RESOURCES_P + vamanaFilename); exists(filePath)) {
		graph = Graph<float>({},L,R,k,a);
		TIMER_BLOCK("Filtered Vamana Index Import",
			graph.importGraph(vamanaFilename);
		)
	}
	else {
		graph = Graph<float>(dataset.getVectors(),L,R,k,a);
		TIMER_BLOCK("Vamana Index build",
			graph.vamana();
		)
		graph.exportGraph(vamanaFilename);
	}

	TIMER_BLOCK("Filter Queries Computation",
		runQueries<float>(graph,querySet,groundtruthSet);
	)

	DIVIDER

}