CXX = g++
CXXFLAGS = -std=c++11
BIN_DIR = bin
SRC_DIR = .
INCLUDE_DIR = include

# Define your source files for the main application
SOURCES = $(SRC_DIR)/benchmark.cpp $(SRC_DIR)/heap.cpp $(SRC_DIR)/quick_sort.cpp
# Define the source file for testing
TEST_SRC = $(SRC_DIR)/tests.cpp

# Define the executables
EXECUTABLE = $(BIN_DIR)/benchmark
TEST_EXECUTABLE = $(BIN_DIR)/tests.cpp

# Default target
all: benchmark test

# Target for the main application
$(EXECUTABLE): $(SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $(SOURCES) -o $@

# Target for the test suite
$(TEST_EXECUTABLE): $(TEST_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $(TEST_SRC) -o $@

# Add a target to clean, build and run the benchmark
benchmark: clean $(EXECUTABLE)
	./$(EXECUTABLE)

# Target to compile and run tests
test: $(TEST_EXECUTABLE)
	./$(TEST_EXECUTABLE)

run: benchmark

clean:
	rm -rf $(BIN_DIR)

.PHONY: all run clean benchmark test

