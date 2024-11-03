# Compiler
CC = g++
CFLAGS = -std=c++20 -I../include -I../src/main/cpp/utility

TEST_SOURCES = test_addVertex.cpp test_addEdge.cpp test_initializeRandomEdge.cpp test_medoid.cpp

TEST_EXECUTABLES = $(patsubst %.cpp,../build/%,$(TEST_SOURCES))

../build/%: %.cpp ../src/main/cpp/utility/Graph/Graph.cpp ../src/main/cpp/utility/Edge/Edge.cpp ../src/main/cpp/utility/DataSet/DataSet.cpp
	$(CC) $(CFLAGS) $< ../src/main/cpp/utility/Graph/Graph.cpp ../src/main/cpp/utility/Edge/Edge.cpp ../src/main/cpp/utility/DataSet/DataSet.cpp -o $@


tests: $(TEST_EXECUTABLES)
	@for test in $(TEST_EXECUTABLES); do \
		echo "Running $$test..."; \
		$$test; \
	done

clean:
	rm -f $(TEST_EXECUTABLE)
