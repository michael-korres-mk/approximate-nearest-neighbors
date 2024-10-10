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

	baseDataSet->print();
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