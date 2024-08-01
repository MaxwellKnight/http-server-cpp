# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude

SRC_DIR = src
OBJ_DIR = build
SOURCES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.cpp)
OBJECTS_CPP = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXECUTABLE = $(OBJ_DIR)/server

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS_CPP)
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS_CPP)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)

.PHONY: all clean
