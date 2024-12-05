include $(INCLUDE)

CXX = g++
CXXFLAGS = -O3 -std=c++17 -g

# Source file directory
SRC_DIR = src/main/cpp

# Directory where object files will be placed
BUILD_DIR = build

# Find all source files recursively
SRCS = $(DEPENDENCIES) $(shell find $(SRC_DIR)/$(TARGET) -name "*.cpp")
# Create a list of corresponding object files in the build directory
# $(patsubst <pattern>, <replacement>, <text>)
# src/main/cpp/foo.o → build/myapp/foo.o
OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.cpp=.o))

# Executable name
EXECUTABLE = $(BUILD_DIR)/$(TARGET)/main

# Target: all (default target)
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target: clean
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: all clean