CXX = g++
CXXFLAGS = -std=c++11
BIN_DIR = bin
SRC_DIR = .
INCLUDE_DIR = include

# Define your source files
SOURCES = $(SRC_DIR)/benchmark.cpp $(SRC_DIR)/heap.cpp $(SRC_DIR)/quick_sort.cpp

# Define the executable
EXECUTABLE = $(BIN_DIR)/benchmark

# Default target
all: benchmark

$(EXECUTABLE): $(SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) $(SOURCES) -o $@

# Add a target to clean, build and run the benchmark
benchmark: clean $(EXECUTABLE)
	./$(EXECUTABLE)

run: benchmark

clean:
	rm -rf $(BIN_DIR)

.PHONY: all run clean benchmark

