# include <iostream>
# include <cstring>
# include "utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
#include <iomanip>


#include "utility/Utils/Utils.h"
#include "utility/VectorFileType/VectorFileType.h"

using namespace std;

void initializeDatasets(DataSet<float>** baseDataSet, DataSet<float>** queryDataSet, DataSet<float>** learningDataSet, DataSet<int>** groundtruthDataSet,char* argv[],int argc) ;


int main(int argc,char* argv[]) {
	// if(argc < 9)exit(EXIT_FAILURE

	DataSet<float>* baseDataSet;
	DataSet<float>* queryDataSet;
	DataSet<float>* learningDataSet;
	DataSet<int>* groundtruthDataSet;

	initializeDatasets(&baseDataSet,&queryDataSet,&learningDataSet,&groundtruthDataSet,argv,argc);


	int numOfQueries = queryDataSet->getNumOfVectors();

	int nearestNeighborId;
	int groundTruthNearestNeighborId;
	for(int i = 0; i < numOfQueries;i++) {
		nearestNeighborId = baseDataSet->getNearestNeighbor(queryDataSet->getVector(i));
		groundTruthNearestNeighborId = groundtruthDataSet->getVector(i)[0];
		if(nearestNeighborId == groundTruthNearestNeighborId) {
			cout << "Q" << i << ": " << "SUCCESS" << endl;
		}
		else {
			cout << "Q" << i << ": " << "FAILURE" << endl;
		}
	}

}

void initializeDatasets(DataSet<float>** baseDataSet, DataSet<float>** queryDataSet, DataSet<float>** learningDataSet, DataSet<int>** groundtruthDataSet,char* argv[],int argc)  {

	char* baseVectorsDataFileName;
	char* queryVectorsDataFileName;
	char* learningVectorsDataFileName;
	char* groundtruthVectorsDataFileName;

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

	*baseDataSet = new DataSet<float>(baseVectorsDataFileName);
	*queryDataSet = new DataSet<float>(queryVectorsDataFileName);
	*groundtruthDataSet = new DataSet<int>(groundtruthVectorsDataFileName);
	*learningDataSet = new DataSet<float>(learningVectorsDataFileName);

}


void printDatasets(DataSet<float>* baseDataSet, DataSet<float>* queryDataSet, DataSet<float>* learningDataSet, DataSet<int>* groundtruthDataSet) {
	cout << endl << "base set | 1st 3 vectors : " << endl;
	for(int i = 0; i < 3; i++) {
		cout << "v{" << i << "}: " << endl;
		for(int j = 0; j < baseDataSet->getD();j++) {
			cout << baseDataSet->getVectors()[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl << "learning set | 1st 3 vectors : " << endl;
	for(int i = 0; i < 3; i++) {
		cout << "v{" << i << "}: " << endl;
		for(int j = 0; j < learningDataSet->getD();j++) {
			cout << learningDataSet->getVectors()[i][j] << " ";
		}
		cout << endl;
	}


	cout << endl << "query set | 1st 3 vectors : " << endl;
	for(int i = 0; i < 3; i++) {
		cout << "v{" << i << "}: " << endl;
		for(int j = 0; j < queryDataSet->getD();j++) {
			cout << queryDataSet->getVectors()[i][j] << " ";
		}
		cout << endl;
	}


	cout << endl << "groundtruth set | 1st 3 vectors : " << endl;
	for(int i = 0; i < 3; i++) {
		cout << "v{" << i << "}:  neighbors: ";
		for(int j = 0; j < groundtruthDataSet->getD();j++) {

			cout << i << "."<< j << "|" <<groundtruthDataSet->getVectors()[i][j] << " ";
		}
		cout << endl;
	}

}