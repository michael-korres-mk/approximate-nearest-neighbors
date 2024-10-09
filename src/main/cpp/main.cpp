# include <iostream>
# include <cstring>
# include "utility/DataSet/DataSet.h"
# include <fcntl.h>
# include <unistd.h>
#include <iomanip>


#include "utility/Utils/Utils.h"

using namespace std;

void initializeDatasets(DataSet** baseDataSet, DataSet** queryDataSet, DataSet** learningDataSet, DataSet** groundtruthDataSet,char* argv[],int argc) ;


int main(int argc,char* argv[]) {
	// if(argc < 9)exit(EXIT_FAILURE);


	DataSet* baseDataSet;
	DataSet* queryDataSet;
	DataSet* learningDataSet;
	DataSet* groundtruthDataSet;

	initializeDatasets(&baseDataSet,&queryDataSet,&learningDataSet,&groundtruthDataSet,argv,argc);


	baseDataSet->print();


}

void initializeDatasets(DataSet** baseDataSet, DataSet** queryDataSet, DataSet** learningDataSet, DataSet** groundtruthDataSet,char* argv[],int argc)  {

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

	*baseDataSet = new DataSet(baseVectorsDataFileName);
	*queryDataSet = new DataSet(queryVectorsDataFileName);
	// *learningDataSet = new DataSet(learningVectorsDataFileName); // it needs implementation for ints
	*groundtruthDataSet = new DataSet(groundtruthVectorsDataFileName);

}