//
// Created by mkorres on 11/10/2024.
//

# include <chrono>
# include <iostream>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
#include <filesystem>
# include "FilterGraph/FilterGraph.h"
# include "FilterDataset/FilterDataset.h"
# include "FilterQuerySet/FilterQuerySet.h"
# include "../utility/Utils/Utils.h"
# include "../utility/DataSet/DataSet.h"

using namespace std;

string dataFilename;
string queriesFileName;
string groundtruthFileName;

int k;
int L;
int R;
double a;

int filteredVamana;

void initializeDatasets(FilterDataset<float>& dataset, FilterQuerySet<float>& querySet,DataSet<int>& groundtruthSet) {

	cout << "Base Dataset: " << dataFilename << endl;
	cout << "Query Dataset: " << queriesFileName << endl;
	cout << "Ground-truth Dataset: " << groundtruthFileName << endl;

	DIVIDER

	TIMER_BLOCK("Base dataset load",
		dataset = FilterDataset<float>(dataFilename);
	)

	cout << "Num of datapoints: " << dataset.numOfDataPoints << endl;

	DIVIDER

	TIMER_BLOCK("Query dataset load",
		querySet = FilterQuerySet<float>(queriesFileName);
	)

	cout << "Num of queries: " << querySet.numOfQueries << endl;

	DIVIDER

	TIMER_BLOCK("Ground-truth dataset load",
		groundtruthSet = DataSet<int>(groundtruthFileName);
	)

}

template <typename T>
void runQueries(FilterGraph<T> fgraph,FilterQuerySet<T> qset,DataSet<int>& groundtruthDataSet) {
	DIVIDER

	const int nq = qset.numOfQueries;

	int filteredQueries = 0;
	int unfilteredQueries = 0;
	int noneighborQueries = 0;
	int otherQueries = 0;

	double totalKRecallFiltered = 0.0;
	double totalKRecallUnfiltered = 0.0;

	map<int,int> filtersStartingPoints = fgraph.findMedoid();
	vector<int> allStartingPoints;
	for (const auto&[_, point] : filtersStartingPoints) allStartingPoints.push_back(point);

	printf("running queries ... \n");
	for(int i = 0; i < nq; i++) {

		if (nq > 10 && i > 0 && i % (nq / 10) == 0) printf("queries completed %d %% ... \n", i * 100 / nq);

		int query_type = qset.queries[i].queryType;
		int v = qset.queries[i].v;

		vector<int> groundTruthNearestNeighbors = groundtruthDataSet.vectors[i];

		if(groundTruthNearestNeighbors[0] == 0) {
			noneighborQueries++;
			continue;
		}

		// resize the vector to cut the 1st element (containing the num of elements) and to stop at the first -1
		groundTruthNearestNeighbors.erase(groundTruthNearestNeighbors.begin());
		auto it = find(groundTruthNearestNeighbors.begin(), groundTruthNearestNeighbors.end(), -1);
		if (it != groundTruthNearestNeighbors.end()) groundTruthNearestNeighbors.resize(distance(groundTruthNearestNeighbors.begin(), it));

		if(query_type == 0){  // only ANN
			unfilteredQueries++;
			const auto& [neighbors,V] = fgraph.filteredGreedySearch(allStartingPoints,qset.queries[i].vec,k,L,-1);
			totalKRecallUnfiltered += FilterGraph<int>::equals(neighbors,groundTruthNearestNeighbors);
		}
		else if(query_type == 1){ // equal + ANN
			filteredQueries++;
			const auto& [neighbors,V] = fgraph.filteredGreedySearch({filtersStartingPoints[v]},qset.queries[i].vec,k,L,v);
			totalKRecallFiltered += FilterGraph<int>::equals(neighbors,groundTruthNearestNeighbors);
		}
		else {
			otherQueries++;
		}
	}

	printf("queries completed 100 %% ... \n");

	PRINT_VAR(filteredQueries)
	PRINT_VAR(unfilteredQueries)
	PRINT_VAR(otherQueries)
	PRINT_VAR(noneighborQueries)


	if(unfilteredQueries == 0) {
		printf("filtered k-recall@k: %.2lf%%\n",(totalKRecallFiltered / filteredQueries) * 100);
		printf("No unfiltered queries\n");
	}
	else if(filteredQueries == 0) {
		printf("unfiltered k-recall@k: %.2lf%%\n",(totalKRecallUnfiltered / unfilteredQueries) * 100);
		printf("No filtered queries\n");
	}
	else {
		printf("filtered k-recall@k: %.2lf%%\n",(totalKRecallFiltered / filteredQueries) * 100);
		printf("unfiltered k-recall@k: %.2lf%%\n",(totalKRecallUnfiltered / unfilteredQueries) * 100);
	}
	DIVIDER
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
		GET_INT_ARG("-fvamana", filteredVamana)
	}


	PRINT_VAR(k)
	PRINT_VAR(L)
	PRINT_VAR(R)
	PRINT_VAR(a)

	DIVIDER

	FilterDataset<float> dataset;
	FilterQuerySet<float> querySet;
	DataSet<int> groundtruthSet;

	initializeDatasets(dataset,querySet,groundtruthSet);

	DIVIDER

	const string filteredVamanaFilename = "filtered_vamana_graph.bin";

	const string stitchedVamanaFilename = "stitched_vamana_graph.bin";

	FilterGraph<float> filteredGraph;

	if(filteredVamana) {
		if(filesystem::path filePath(RESOURCES_P + filteredVamanaFilename); exists(filePath)) {
			filteredGraph = FilterGraph<float>({},L,R,k,a, 10);
			TIMER_BLOCK("Filtered Vamana Index Import",
				filteredGraph.importFilterGraph(filteredVamanaFilename);
			)
		}
		else {
			filteredGraph = FilterGraph<float>(dataset.datapoints,L,R,k,a, 10);
			TIMER_BLOCK("Filtered Vamana Index build",
				filteredGraph.filteredVamana();
			)
			filteredGraph.exportFilterGraph(filteredVamanaFilename);
		}
	}
	else {
		if(filesystem::path filePath(RESOURCES_P + stitchedVamanaFilename); exists(filePath)) {
			filteredGraph = FilterGraph<float>({},L,R,k,a, 10);
			TIMER_BLOCK("Stitched Vamana Index Import",
				filteredGraph.importFilterGraph(stitchedVamanaFilename);
			)
		}
		else {
			filteredGraph = FilterGraph<float>(dataset.datapoints,L,R,k,a, 10);
			TIMER_BLOCK("Stitched Vamana Index build",
				filteredGraph.stitchedVamana();
			)
			filteredGraph.exportFilterGraph(stitchedVamanaFilename);
		}
	}

	TIMER_BLOCK("Filter Queries Computation",
		runQueries<float>(filteredGraph,querySet,groundtruthSet);
	)

	DIVIDER
}
