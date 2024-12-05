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

void initializeDatasets(FilterDataset<float>& dataset, FilterQuerySet<float>& querySet,char* argv[],int argc) ;


int k;
int L;
int R;
double a;

int main(int argc,char* argv[]) {
	Utils<char>::printDivider();

	FilterDataset<float> dataset;
	FilterQuerySet<float> querySet;

	initializeDatasets(dataset,querySet,argv,argc);

	DIVIDER

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

	DIVIDER

    // FilterGraph<float> graph(dataset.dataPoints,L,R,k,a, 10);
    //
    // cout << "Start findMedoid()" << endl;
    // map<int,int> m = graph.findMedoid();
    // cout << "Finish findMedoid()" << endl;


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

	TIMER_BLOCK("Base dataset load",
		dataset = FilterDataset<float>(baseVectorsDataFileName);
	)

	cout << "Num of datapoints: " << dataset.numOfDataPoints << endl;

	Utils<char>::printDivider();

	TIMER_BLOCK("Query dataset load",
		querySet = FilterQuerySet<float>(queryVectorsDataFileName);
	)

	cout << "Num of queries: " << querySet.numOfQueries << endl;

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