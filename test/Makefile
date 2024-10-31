# Compiler
CC = g++
CFLAGS = -std=c++20 -I../include -I../src/main/cpp/utility

TEST_SOURCES = graph_test.cpp ../src/main/cpp/utility/Graph/Graph.cpp ../src/main/cpp/utility/Edge/Edge.cpp ../src/main/cpp/utility/DataSet/DataSet.cpp
TEST_EXECUTABLE = ../build/graph_test

all: $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(TEST_SOURCES) -o $(TEST_EXECUTABLE)

tests: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

clean:
	rm -f $(TEST_EXECUTABLE)
