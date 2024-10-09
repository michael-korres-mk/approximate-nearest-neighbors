all:
	$(MAKE) -f make-ann

# Target: clean
clean:
	$(MAKE) clean -f make-ann
#	rm -f $(shell find $(RESOURCES_DIR) -type f ! -name 'voters*0.bin')

.PHONY: all clean

# Directory where object files will be placed
BUILD_DIR = build

# Directory where resource files will be placed
RESOURCES_DIR = src/main/resources

DATASET = siftsmall
BASE_DATA_FILE = $(DATASET)/$(DATASET)_base.fvecs
GROUNDTRUTH_DATA_FILE = $(DATASET)/$(DATASET)_groundtruth.ivecs
LEARN_DATA_FILE = $(DATASET)/$(DATASET)_learn.fvecs
QUERY_DATA_FILE = $(DATASET)/$(DATASET)_query.fvecs

main:
	$(BUILD_DIR)/ann -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -lv $(LEARN_DATA_FILE) -qv $(QUERY_DATA_FILE)


valgrind-ann:
	valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no 	$(BUILD_DIR)/ann -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -lv $(LEARN_DATA_FILE) -qv $(QUERY_DATA_FILE)

