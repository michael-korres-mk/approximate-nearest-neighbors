# Include external parameter file
include configs/ann.mk
include configs/filterann.mk
include configs/annimport.mk

all:
	$(MAKE) -f makefiles/make-ann.mk
	$(MAKE) -f makefiles/make-annimport.mk
	$(MAKE) -f makefiles/make-filterann.mk
	$(MAKE) -f makefiles/make-test.mk

ann-build:
	$(MAKE) -f makefiles/make-ann.mk

annimport-build:
	$(MAKE) -f makefiles/make-annimport.mk

filterann-build:
	$(MAKE) -f makefiles/make-filterann.mk

tests-build:
	$(MAKE) -f makefiles/make-test.mk

# Target: clean
clean:
	$(MAKE) clean -f makefiles/make-ann.mk
	$(MAKE) clean -f makefiles/make-filterann.mk
	$(MAKE) clean -f makefiles/make-annimport.mk
	$(MAKE) clean -f makefiles/make-test.mk
#	rm -f $(shell find $(RESOURCES_DIR) -type f ! -name 'voters*0.bin')

.PHONY: all clean

# Directory where object files will be placed
BUILD_DIR = build

# Directory where resource files will be placed
RESOURCES_DIR = src/main/resources

ann:
	$(BUILD_DIR)/ann -k $(ANN_k) -L $(ANN_L) -R $(ANN_R) -a $(ANN_a) -bv $(ANN_BASE_DATA_FILE) -gv $(ANN_GROUNDTRUTH_DATA_FILE) -qv $(ANN_QUERY_DATA_FILE)

annimport:
	$(BUILD_DIR)/annimport -k $(ANNIMPORT_k) -L $(ANNIMPORT_L) -R $(ANNIMPORT_R) -a $(ANNIMPORT_a) -bv $(ANNIMPORT_BASE_DATA_FILE) -gv $(ANNIMPORT_GROUNDTRUTH_DATA_FILE) -qv $(ANNIMPORT_QUERY_DATA_FILE)

filterann:
	$(BUILD_DIR)/filterann -k $(FILTERANN_k) -L $(FILTERANN_L) -R $(FILTERANN_R) -a $(FILTERANN_a) -bv $(FILTERANN_DATASET_FILE) -qv $(FILTERANN_QUERY_SET_FILE)


TEST_SOURCES = $(wildcard tests/*)
TEST_EXECUTABLES = $(patsubst %.cpp,build/%,$(TEST_SOURCES))

tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test"; \
		$$test; \
	done


valgrind-ann:
	valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no 	$(BUILD_DIR)/ann -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -qv $(QUERY_DATA_FILE)

valgrind-annimport:
	valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no $(BUILD_DIR)/annimport -k $(ANN_k) -L $(ANN_L) -R $(ANN_R) -a $(ANN_a) -bv $(ANN_BASE_DATA_FILE) -gv $(ANN_GROUNDTRUTH_DATA_FILE) -qv $(ANN_QUERY_DATA_FILE)

valgrind-filterann:
	valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no $(BUILD_DIR)/filterann -k $(FILTERANN_k) -L $(FILTERANN_L) -R $(FILTERANN_R) -a $(FILTERANN_a) -bv $(FILTERANN_DATASET_FILE) -qv $(FILTERANN_QUERY_SET_FILE)

valgrind-tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test"; \
		valgrind --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no $$test; \
	done