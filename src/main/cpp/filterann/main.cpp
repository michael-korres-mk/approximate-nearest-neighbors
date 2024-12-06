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

#define DIVIDER Utils<char>::printDivider();

using namespace std;

string dataFilename;
string queriesFileName;
string groundtruthFileName;

int k;
int L;
int R;
double a;

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

	double totalKRecall = 0.0;

	map<int,int> filtersStartingPoints = fgraph.findMedoid();
	vector<int> allStartingPoints;
	for (const auto&[_, point] : filtersStartingPoints) allStartingPoints.push_back(point);

	for(int i = 0; i < nq; i++) {
		int query_type = qset.queries[i].queryType;
		int v = qset.queries[i].v;


		if(query_type == 0){  // only ANN
			const auto& [neighbors,V] = fgraph.filteredGreedySearch(allStartingPoints,qset.queries[i].vec,k,L,-1);
			// if((Fq == -1) || vertexMap[s].C == Fq) // if unfiltered query or s categorical attribute matches,add to l
			vector<int> groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
			totalKRecall += FilterGraph<int>::equals(neighbors,groundTruthNearestNeighbors);
		}
		else if(query_type == 1){ // equal + ANN
			const auto& [neighbors,V] = fgraph.filteredGreedySearch({filtersStartingPoints[v]},qset.queries[i].vec,k,L,v);
			vector<int> groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
			totalKRecall += FilterGraph<int>::equals(neighbors,groundTruthNearestNeighbors);
		}
	}

	PRINT_VAR(totalKRecall)
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

	FilterDataset<float> dataset;
	FilterQuerySet<float> querySet;
	DataSet<int> groundtruthSet;

	initializeDatasets(dataset,querySet,groundtruthSet);


	DIVIDER

	const string filteredVamanaFilename = "filtered_vamana_graph.bin";

	const string stitchedVamanaFilename = "stitched_vamana_graph.bin";


	FilterGraph<float> filteredGraph;

	bool filteredVamana = true;

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

	runQueries<float>(filteredGraph,querySet,groundtruthSet);

}
