# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Ensure the bin directory exists and specify output directory for executables
build:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/processing processing.cpp

# Compile and run the main program
run: build
	./bin/processing

# Compile and run the test suite
test:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o bin/suite processing.cpp suite.cpp
	./bin/suite

# Clean up executables
clean:
	rm -f bin/processing bin/suite
	rmdir bin
