all:
	$(MAKE) -f make-ann

annimport-build:
	$(MAKE) -f make-annimport

tests-build:
	$(MAKE) -f make-test

# Target: clean
clean:
	$(MAKE) clean -f make-ann
	$(MAKE) clean -f make-test
#	rm -f $(shell find $(RESOURCES_DIR) -type f ! -name 'voters*0.bin')

.PHONY: all clean

# Directory where object files will be placed
BUILD_DIR = build

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

ann:
	$(BUILD_DIR)/ann -k $(k) -L $(L) -R $(R) -a $(a) -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -qv $(QUERY_DATA_FILE)

annimport:
	$(BUILD_DIR)/annimport -k $(k) -L $(L) -R $(R) -a $(a) -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -qv $(QUERY_DATA_FILE)


TEST_SOURCES = $(wildcard tests/*)
TEST_EXECUTABLES = $(patsubst %.cpp,build/%,$(TEST_SOURCES))

tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test"; \
		$$test; \
	done


valgrind-ann:
	valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no 	$(BUILD_DIR)/ann -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -qv $(QUERY_DATA_FILE)