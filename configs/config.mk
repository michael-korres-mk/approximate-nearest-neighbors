# Directory where resource files will be placed
RESOURCES_DIR = src/main/resources

DATASET = siftsmall
BASE_DATA_FILE = $(DATASET)/$(DATASET)_base.fvecs
GROUNDTRUTH_DATA_FILE = $(DATASET)/$(DATASET)_groundtruth.ivecs
QUERY_DATA_FILE = $(DATASET)/$(DATASET)_query.fvecs

k = 100
L = 250
R = 60
a = 1.2
