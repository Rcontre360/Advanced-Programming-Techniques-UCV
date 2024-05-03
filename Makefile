CXX = g++
CXXFLAGS = -I./$(PROJECT) -std=c++11
BIN_DIR = bin

test: clean build
	./$(BIN_DIR)/test_suite $(PROJECT)

build: $(BIN_DIR)/suite.o $(BIN_DIR)/processing.o
	$(CXX) -o $(BIN_DIR)/test_suite $(BIN_DIR)/suite.o $(BIN_DIR)/processing.o

$(BIN_DIR)/suite.o: suite.cpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c suite.cpp -o $(BIN_DIR)/suite.o

$(BIN_DIR)/processing.o: $(PROJECT)/processing.cpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $(PROJECT)/processing.cpp -o $(BIN_DIR)/processing.o

clean:
	rm -rf $(BIN_DIR)

