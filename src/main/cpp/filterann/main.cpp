//
// Created by mkorres on 11/10/2024.
//

# include <chrono>
# include <iostream>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
# include "FilterGraph/FilterGraph.h"
# include "FilterDataset/FilterDataset.h"
# include "FilterQuerySet/FilterQuerySet.h"
# include "../utility/Utils/Utils.h"
# include "../utility/DataSet/DataSet.h"

#define DIVIDER Utils<char>::printDivider();



using namespace std;

void initializeDatasets(FilterDataset<float>& dataset, FilterQuerySet<float>& querySet,DataSet<int> groundtruthSet) ;


string dataFilename;
string queriesFileName;
string groundtruthFileName;

int k;
int L;
int R;
double a;

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

    // FilterGraph<float> graph(dataset.dataPoints,L,R,k,a, 10);
    //
    // cout << "Start findMedoid()" << endl;
    // map<int,int> m = graph.findMedoid();
    // cout << "Finish findMedoid()" << endl;


}

void initializeDatasets(FilterDataset<float>& dataset, FilterQuerySet<float>& querySet,DataSet<int> groundtruthSet) {

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
	int nq = qset.numOfQueries;

	double totalKRecall = 0.0;
	for(int i = 0; i < nq; i++) {
		int query_type = qset.queries[i].queryType;
		int v = qset.queries[i].v;

		if(query_type == 0){  // only ANN
			const auto& [neighbors,V] = fgraph.filteredGreedySearch({},qset.queries[i].vec,k,L,-1);
			vector<int> groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
			double kRecall = FilterGraph<int>::equals(neighbors,groundTruthNearestNeighbors);
			totalKRecall += kRecall;
		}
		else if(query_type == 1){ // equal + ANN
			const auto& [neighbors,V] = fgraph.filteredGreedySearch({},qset.queries[i].vec,k,L,v);
			vector<int> groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
			double kRecall = FilterGraph<int>::equals(neighbors,groundTruthNearestNeighbors);
			totalKRecall += kRecall;
		}
	}
}