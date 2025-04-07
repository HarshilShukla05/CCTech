# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Igeometry/include

# Directories
BIN_DIR = bin
DATA_DIR = data
SRC_DIR = application/src
GEOM_SRC_DIR = geometry/src
GEOM_INC_DIR = geometry/include

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(GEOM_SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst %.cpp, %.o, $(SRC_FILES))

# Output executable
TARGET = $(BIN_DIR)/shape_plotter.exe

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJ_FILES) | $(BIN_DIR) $(DATA_DIR)
	@echo Linking and building executable...
	$(CXX) $(OBJ_FILES) -o $(TARGET) -lm

# Compiling
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if not exist
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(DATA_DIR):
	@mkdir -p $(DATA_DIR)

# Clean command - POSIX compatible (works in MSYS2 and Git Bash)
clean:
	@echo Cleaning build artifacts...
	@rm -f $(OBJ_FILES) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

# Move STL plot file to data folder
stl_plot:
	@mv -f stl_plot.dat $(DATA_DIR) 2>/dev/null || true

# Declare phony targets
.PHONY: all clean run stl_plot
