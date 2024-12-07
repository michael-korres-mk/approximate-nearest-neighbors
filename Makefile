# Include external parameter file
include configs/ann.mk.conf
include configs/filterann.mk.conf
include configs/groundtruthcalc.mk.conf

BUILD_DIR = build

TARGETS = ann filterann groundtruthcalc
TESTS = $(patsubst src/tests/cpp/%.cpp,build/tests/%,$(shell find src/tests/cpp -name "*.cpp"))

b: $(addsuffix b, $(TARGETS))
	@:

$(addsuffix b, $(TARGETS)):
	@clear
	$(MAKE) INCLUDE=configs/$(@:b=).mk.conf -f makefiles/template.mk

testb:
	@clear
	$(MAKE) -f makefiles/make-test.mk

$(TARGETS):
	@clear
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

$(addsuffix v, $(TARGETS)):
	@clear
	valgrind $(VALGRIND_FLAGS) $(BUILD_DIR)/$(@:v=)/main $($(shell echo "$@" | tr 'a-z' 'A-Z')_CLINE_ARGS)

valgrind-tests: $(TEST_EXECUTABLES)
	@clear
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test"; \
		valgrind $(VALGRIND_FLAGS) $$test; \
	done