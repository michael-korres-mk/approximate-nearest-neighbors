# Compiler
CXX = g++
CXXFLAGS = -O2 -std=c++17 -g

SRCS = src/main/cpp

# Find all test sources
TEST_SOURCES = $(wildcard tests/*.cpp)
TEST_EXECUTABLES = $(patsubst tests/%.cpp, build/tests/%,$(TEST_SOURCES))

# Rule to create each test executable
build/tests/%: tests/%.cpp $(wildcard $(SRCS)/utility/**/*.cpp)
	@mkdir -p build/tests # Ensure the build/tests directory exists
	$(CXX) $(CXXFLAGS) $< $(filter-out $(SRCS)/utility/*/ann.cpp, $(wildcard $(SRCS)/utility/**/*.cpp)) -o $@

# Target: all (default target)
all: $(TEST_EXECUTABLES)

# Run tests
tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test"; \
		$$test; \
	done

# Clean up build directory
clean:
	rm -rf build/tests/*
