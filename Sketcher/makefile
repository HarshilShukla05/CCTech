# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -Igeometry/include

# Directories
BIN_DIR = bin
DATA_DIR = data
SRC_DIR = application/src
GEOM_SRC_DIR = geometry/src
GEOM_INC_DIR = geometry/include

# Automatically detect all .cpp files in the source directories
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp $(GEOM_SRC_DIR)/*.cpp)

# Generate object files from the source files
OBJ_FILES = $(patsubst %.cpp, %.o, $(SRC_FILES))

# Target executable
TARGET = $(BIN_DIR)/shape_plotter

# Default rule to build the project
all: $(TARGET)

# Rule to link the final executable
$(TARGET): $(OBJ_FILES) | $(BIN_DIR) $(DATA_DIR)
	@echo "Linking and building the executable..."
	@/usr/bin/time -f "\nBuild Time: %E" $(CXX) $(OBJ_FILES) -o $(TARGET) -lm

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create bin and data directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(DATA_DIR):
	mkdir -p $(DATA_DIR)

# Clean rule to remove all generated files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Run the project
run: $(TARGET) | $(DATA_DIR)
	./$(TARGET)

# Store STL plot data in the bin directory
stl_plot:
	mv stl_plot.dat $(DATA_DIR)/