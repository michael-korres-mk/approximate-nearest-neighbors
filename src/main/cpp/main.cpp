# include <iostream>
# include <cstring>
# include "utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>
#include "utility/Graph/Graph.h"
# include "utility/Utils/Utils.h"

using namespace std;

void initializeDatasets(DataSet<float>& baseDataSet, DataSet<float>& queryDataSet, DataSet<int>& groundtruthDataSet,char* argv[],int argc);
tuple<int, int, double, int> calculate_parameters(int num_nodes);

int main(int argc,char* argv[]) {
	// if(argc < 9)exit(EXIT_FAILURE

	DataSet<float> baseDataSet;
	DataSet<float> queryDataSet;
	DataSet<int> groundtruthDataSet;

	initializeDatasets(baseDataSet,queryDataSet,groundtruthDataSet,argv,argc);


	// int numOfQueries = queryDataSet.getNumOfVectors();
	// int k = groundtruthDataSet.getD();
	// int R = 50;
	// double a = 1.8;

	int num_nodes = 1000;

	vector<vector<float>> subset;
	for(int i = 0; i < num_nodes; i++)subset.push_back(baseDataSet.getVector(i));

	auto [R, k, a, L] = calculate_parameters(num_nodes);
	cout << "Για " << num_nodes << " nodes:\n";
	cout << "R = " << R << "\n";
	cout << "k = " << k << "\n";
	cout << "a = " << a << "\n";
	cout << "L = " << L << "\n";

	Graph graph(subset,k,R,a,L);

}

tuple<int, int, double, int> calculate_parameters(int num_nodes) {
	int R = 10 + 10 * std::log10(num_nodes);                   // Μέγιστος αριθμός γειτόνων
	int k = 5 + std::sqrt(num_nodes / 1000.0);                 // Αριθμός γειτόνων προς αναζήτηση
	double a = 1.2 + 0.1 * std::log10(num_nodes);              // Κατώφλι απόστασης
	int L = 2 * k + num_nodes / 1000;                          // Μέγεθος λίστας αναζήτησης

	return {R, k, a, L};
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