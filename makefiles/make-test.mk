# Compiler
CXX = g++
CXXFLAGS = -O3 -std=c++17 -g -fopenmp # acutest does not work with: -Wall -Wextra -Werror -pedantic

SRC_DIR = src/main/cpp
TEST_DIR = src/tests/cpp
BUILD_DIR = build
TEST_BUILD_DIR = build/tests

# Find all test sources

TEST_SRCS =  $(shell find $(TEST_DIR) -name "*.cpp")
ALL_SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
MAINS := $(shell find $(SRC_DIR) -name "main.cpp")

# $(filter-out <pattern>, <text>)
FILTERED_SRCS = $(filter-out $(MAINS), $(ALL_SRCS))

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

FILTERED_OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(FILTERED_SRCS:.cpp=.o))

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

TEST_OBJS = $(patsubst $(TEST_DIR)/%,$(TEST_BUILD_DIR)/%,$(TEST_SRCS:.cpp=.o))

# Rule to create each test executable
$(TEST_BUILD_DIR)/%: $(TEST_BUILD_DIR)/%.o
	@mkdir -p $(@D)  # Ensure build directory exists
	$(CXX) $(CXXFLAGS) $< $(FILTERED_OBJS) -o $@


TEST_EXECUTABLES = $(patsubst $(TEST_DIR)/%,$(TEST_BUILD_DIR)/%,$(TEST_SRCS:.cpp=))


all: $(TEST_EXECUTABLES)

# Clean up build directory
clean:
	rm -rf build/tests/*
