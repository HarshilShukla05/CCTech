# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++11 -Igeometry/include

# Directories
BIN_DIR = bin
DATA_DIR = data
SRC_DIR = application/src
GEOM_SRC_DIR = geometry/src
GEOM_INC_DIR = geometry/include

# Object files
OBJ = $(SRC_DIR)/main.o $(GEOM_SRC_DIR)/cuboid.o $(GEOM_SRC_DIR)/cylinder.o $(GEOM_SRC_DIR)/sphere.o $(GEOM_SRC_DIR)/polyline.o $(GEOM_SRC_DIR)/bezier.o $(GEOM_SRC_DIR)/triangle.o $(GEOM_SRC_DIR)/polygon.o $(GEOM_SRC_DIR)/line3D.o $(GEOM_SRC_DIR)/structure.o $(GEOM_SRC_DIR)/plot_utils.o

# Executable
TARGET = $(BIN_DIR)/shape_plotter

# Build the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) -lm

# Compile main.cpp
$(SRC_DIR)/main.o: $(SRC_DIR)/main.cpp $(GEOM_INC_DIR)/shapes.h $(GEOM_INC_DIR)/structure.h
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(SRC_DIR)/main.o

# Compile cuboid.cpp
$(GEOM_SRC_DIR)/cuboid.o: $(GEOM_SRC_DIR)/cuboid.cpp $(GEOM_INC_DIR)/cuboid.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/cuboid.cpp -o $(GEOM_SRC_DIR)/cuboid.o

# Compile cylinder.cpp
$(GEOM_SRC_DIR)/cylinder.o: $(GEOM_SRC_DIR)/cylinder.cpp $(GEOM_INC_DIR)/cylinder.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/cylinder.cpp -o $(GEOM_SRC_DIR)/cylinder.o

# Compile sphere.cpp
$(GEOM_SRC_DIR)/sphere.o: $(GEOM_SRC_DIR)/sphere.cpp $(GEOM_INC_DIR)/sphere.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/sphere.cpp -o $(GEOM_SRC_DIR)/sphere.o

# Compile polyline.cpp
$(GEOM_SRC_DIR)/polyline.o: $(GEOM_SRC_DIR)/polyline.cpp $(GEOM_INC_DIR)/polyline.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/polyline.cpp -o $(GEOM_SRC_DIR)/polyline.o

# Compile bezier.cpp
$(GEOM_SRC_DIR)/bezier.o: $(GEOM_SRC_DIR)/bezier.cpp $(GEOM_INC_DIR)/bezier.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/bezier.cpp -o $(GEOM_SRC_DIR)/bezier.o

# Compile triangle.cpp
$(GEOM_SRC_DIR)/triangle.o: $(GEOM_SRC_DIR)/triangle.cpp $(GEOM_INC_DIR)/triangle.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/triangle.cpp -o $(GEOM_SRC_DIR)/triangle.o

# Compile polygon.cpp
$(GEOM_SRC_DIR)/polygon.o: $(GEOM_SRC_DIR)/polygon.cpp $(GEOM_INC_DIR)/polygon.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/polygon.cpp -o $(GEOM_SRC_DIR)/polygon.o

# Compile line3D.cpp
$(GEOM_SRC_DIR)/line3D.o: $(GEOM_SRC_DIR)/line3D.cpp $(GEOM_INC_DIR)/line3D.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/line3D.cpp -o $(GEOM_SRC_DIR)/line3D.o

# Compile structure.cpp
$(GEOM_SRC_DIR)/structure.o: $(GEOM_SRC_DIR)/structure.cpp $(GEOM_INC_DIR)/structure.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/structure.cpp -o $(GEOM_SRC_DIR)/structure.o

# Compile plot_utils.cpp
$(GEOM_SRC_DIR)/plot_utils.o: $(GEOM_SRC_DIR)/plot_utils.cpp $(GEOM_INC_DIR)/plot_utils.h
	$(CXX) $(CXXFLAGS) -c $(GEOM_SRC_DIR)/plot_utils.cpp -o $(GEOM_SRC_DIR)/plot_utils.o

# Clean build files
clean:
	rm -f $(SRC_DIR)/*.o $(GEOM_SRC_DIR)/*.o $(TARGET)

# Build and run the project
run: $(TARGET)
	./$(TARGET)
