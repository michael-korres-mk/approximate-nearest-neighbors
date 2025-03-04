include $(wildcard configs/*.mk.conf)

RESOURCES_DIR = src/main/resources
BUILD_DIR = build

TARGETS = ann filterann groundtruthcalc
TESTS = $(patsubst src/tests/cpp/%.cpp,build/tests/%,$(shell find src/tests/cpp -name "*.cpp"))

RESULTS_DIR = experiment-results
PLOTS_DIR = experiment-plots
THREAD_NUMS = 0 2 4 8
EXPERIMENT_REPETITIONS = 2


b: $(addsuffix b, $(TARGETS))
	@:

$(addsuffix b, $(TARGETS)):
	@clear
	$(MAKE) INCLUDE=configs/$(@:b=).mk.conf -f makefiles/template.mk

testb:
	@clear
	$(MAKE) -f makefiles/make-test.mk

$(TARGETS):
	$(BUILD_DIR)/$@/main $($(shell echo "$@" | tr 'a-z' 'A-Z')_CLINE_ARGS)

test: $(TESTS)
	@clear
	@for test in $(TESTS); do \
		echo "------------------------------ Running $$test ------------------------------"; \
		$$test; \
	done

clean:
	@clear
	@for target in $(TARGETS); do \
		$(MAKE) clean INCLUDE=configs/$$target.mk.conf -f makefiles/template.mk; \
	done

VALGRIND_FLAGS = --trace-children=yes --leak-check=full --show-leak-kinds=all  --leak-resolution=med --track-origins=yes --vgdb=no

# TODO: checkout running w/ valgrind error messages
$(addsuffix v, $(TARGETS)):
	@clear
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(@:v=)/main $($(shell echo "$(@:v=)" | tr 'a-z' 'A-Z')_CLINE_ARGS)

testv: $(TESTS)
	@clear
	@for test in $(TESTS); do \
		echo "------------------------------ Running $$test ------------------------------"; \
		valgrind $(VALGRIND_FLAGS) $$test; \
	done

$(addsuffix ex, $(TARGETS)): results_dir
	@echo "- Running experiments for $@";
	@for er in $(shell seq 1 $(EXPERIMENT_REPETITIONS)); do \
		echo "--- Repetition $$er";  \
		for tn in $(THREAD_NUMS); do  \
			echo "------ thread num: $$tn";\
			$(MAKE) "$(subst ex,,$@)" $@ NUM_OF_THREADS=$$tn -f makefiles/single-run.mk; \
			echo "---------------------------------------------" ; \
		done; \
	done


results_dir:
	mkdir -p $(RESULTS_DIR)

plots_dir:
	mkdir -p $(PLOTS_DIR)

install-dependencies:
	@echo "Checking if pip is installed..."
	@if ! command -v pip &> /dev/null; then \
	    echo "pip not found. Installing pip..."; \
	    python3 -m ensurepip --upgrade || python -m ensurepip --upgrade; \
	    python3 -m pip install --upgrade pip || python -m pip install --upgrade pip; \
	fi
	@echo "Installing required Python packages..."
	@pip install --user pandas numpy matplotlib
	@echo "Dependencies installed successfully."

p: plots_dir $(addsuffix p, $(TARGETS))
	@:


# plot
$(addsuffix p, $(TARGETS)): plots_dir
	python3 plot-scripts/$(@:p=).py