include $(INCLUDE)

CXX = g++
CXXFLAGS = -O2 -std=c++17 -g

# Source file directory
SRC_DIR = src/main/cpp

# Directory where object files will be placed
BUILD_DIR = build

# Find all source files recursively
SRCS = $(shell find $(SRC_DIR)/$(TARGET) -name "*.c") $(DEPENDENCIES)
# Create a list of corresponding object files in the build directory
OBJS = $(patsubst $(SRC_DIR)%.c,$(BUILD_DIR)/%.o,$(SRCS))

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