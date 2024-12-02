# Include external parameter file
include configs/ann.mk.conf
include configs/filterann.mk

TARGETS = ann annimport filterann

$(addsuffix b, $(TARGETS)):
	$(MAKE) INCLUDE=configs/$(@:b=).mk.conf -f makefiles/template.mk

testb:
	$(MAKE) -f makefiles/make-test.mk



# Target: clean
clean:
	@for target in $(TARGETS); do \
		$(MAKE) clean INCLUDE=configs/$$target.mk.conf -f makefiles/template.mk; \
	done

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

VALGRIND_FLAGS = --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no

valgrind-ann:
	valgrind $(VALGRIND_FLAGS) 	$(BUILD_DIR)/ann -bv $(BASE_DATA_FILE) -gv $(GROUNDTRUTH_DATA_FILE) -qv $(QUERY_DATA_FILE)

valgrind-annimport:
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/annimport -k $(ANN_k) -L $(ANN_L) -R $(ANN_R) -a $(ANN_a) -bv $(ANN_BASE_DATA_FILE) -gv $(ANN_GROUNDTRUTH_DATA_FILE) -qv $(ANN_QUERY_DATA_FILE)

valgrind-filterann:
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/filterann -k $(FILTERANN_k) -L $(FILTERANN_L) -R $(FILTERANN_R) -a $(FILTERANN_a) -bv $(FILTERANN_DATASET_FILE) -qv $(FILTERANN_QUERY_SET_FILE)

valgrind-tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test"; \
		valgrind $(VALGRIND_FLAGS) $$test; \
	done