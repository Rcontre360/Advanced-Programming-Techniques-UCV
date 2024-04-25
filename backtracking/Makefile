CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Use 'make TEST_ARGS="--validate"' to enable validation
TEST_ARGS = "--validate"

# Ensure the bin directory exists and specify output directory for executables
build:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/processing processing.cpp

# Compile and run the main program
run: build
	./bin/processing

# Compile and run the test suite with optional arguments
test:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/suite processing.cpp suite.cpp
	./bin/suite $(TEST_ARGS)

# Clean up executables
clean:
	rm -f bin/processing bin/suite
	rmdir bin

