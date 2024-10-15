# include <iostream>
# include <cstring>
# include "utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
# include <iomanip>


# include "utility/Utils/Utils.h"
# include "utility/VectorFileType/VectorFileType.h"

using namespace std;

void initializeDatasets(DataSet<float>& baseDataSet, DataSet<float>& queryDataSet, DataSet<float>& learningDataSet, DataSet<int>& groundtruthDataSet,char* argv[],int argc) ;


int main(int argc,char* argv[]) {
	// if(argc < 9)exit(EXIT_FAILURE

	DataSet<float> baseDataSet;
	DataSet<float> queryDataSet;
	DataSet<float> learningDataSet;
	DataSet<int> groundtruthDataSet;

	initializeDatasets(baseDataSet,queryDataSet,learningDataSet,groundtruthDataSet,argv,argc);

	// Graph(baseDataSet)

	// int numOfQueries = queryDataSet.getNumOfVectors();
	// int k = groundtruthDataSet.getD();
	//
	// vector<int> nearestNeighbors;
	// vector<int> groundTruthNearestNeighbors;
	//
	// int misses = 0;
	//
	// vector<float> q;
	// for(int i = 0; i < numOfQueries;i++) {
	// 	q = queryDataSet.getVector(i);
	// 	nearestNeighbors = baseDataSet.getNearestNeighbors(q,k);
	// 	groundTruthNearestNeighbors = groundtruthDataSet.getVector(i);
	// 	bool equalVecs = DataSet<int>::equals(nearestNeighbors,groundTruthNearestNeighbors);
	//
	// 	if(equalVecs) {
	// 		cout << "Q" << i << ": " << "SUCCESS" << endl;
	// 	}
	// 	else {
	// 		cout << "Q" << i << ": " << "FAILURE" << endl;
	// 		misses++;
	// 	}
	// }
	//
	// if(misses > 0) cout << "misses: " << misses << endl;
}

void initializeDatasets(DataSet<float>& baseDataSet, DataSet<float>& queryDataSet, DataSet<float>& learningDataSet, DataSet<int>& groundtruthDataSet,char* argv[],int argc)  {

	string baseVectorsDataFileName;
	string queryVectorsDataFileName;
	string learningVectorsDataFileName;
	string groundtruthVectorsDataFileName;

	for(int i = 1; i < argc;i++){	// Get arguments
		if (strcmp(argv[i],"-bv") == 0) {
			baseVectorsDataFileName = argv[i+1];
		} else if (strcmp(argv[i],"-qv") == 0) {
			queryVectorsDataFileName = argv[i+1];
		} else if (strcmp(argv[i],"-lv") == 0) {
			learningVectorsDataFileName = argv[i+1];
		} else if (strcmp(argv[i],"-gv") == 0) {
			groundtruthVectorsDataFileName = argv[i+1];
		}
	}

	baseDataSet = DataSet<float>(baseVectorsDataFileName);
	queryDataSet = DataSet<float>(queryVectorsDataFileName);
	groundtruthDataSet = DataSet<int>(groundtruthVectorsDataFileName);
	learningDataSet = DataSet<float>(learningVectorsDataFileName);

}