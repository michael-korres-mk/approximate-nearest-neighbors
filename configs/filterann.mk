# build config
TARGET = filterann
DEPENDENCIES = $(SRC_DIR)/utility/*.cpp

# arguments
FILTERANN_K = 100
FILTERANN_L = 250
FILTERANN_R = 60
FILTERANN_A = 1.2

FILTERANN_DATASET_FILE = contest-data-release-1m.bin
FILTERANN_GROUNDTRUTH_FILE = mygroundtruth.bin
FILTERANN_QUERY_SET_FILE = contest-queries-release-1m.bin

# command line arguments
FILTERANN_CLINE_ARGS =
	-k $(FILTERANN_K) \
	-L $(FILTERANN_L) \
	-R $(FILTERANN_R) \
	-a $(FILTERANN_A) \
	-bv $(FILTERANN_DATASET_FILE) \
	-gv $(FILTERANN_GROUNDTRUTH_FILE) \
	-qv $(FILTERANN_QUERY_SET_FILE)
