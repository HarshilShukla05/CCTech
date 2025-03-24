# # Compiler and flags
# CXX = g++
# CXXFLAGS = -Wall -I./include -I../geometry/include  # Include geometry headers

# # Directories
# SRC_DIR = src
# OBJ_DIR = obj
# GEOMETRY_DIR = ../geometry  # Geometry directory
# BIN_DIR = ../bin
# LIB_DIR = $(GEOMETRY_DIR)/src  # Change this to where libmylib.so is actually located

# # Output executable
# TARGET = $(BIN_DIR)/application  # Path to the final application

# # Find all the source files in the src directory
# SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# # Replace .cpp with .o to create object files
# OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# # Default rule to build the application
# all: $(TARGET)

# # Ensure geometry is built first, then compile the application
# $(TARGET): geometry $(OBJS)
# 	$(CXX) -o $@ $(OBJS) -L$(LIB_DIR) -lmylib  # Link against the correct library directory

# # Rule to compile each source file into an object file
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
# 	$(CXX) -c $(CXXFLAGS) $< -o $@  # Compile source to object file

# # Ensure obj directory exists
# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# # Rule to build the geometry library by invoking its Makefile
# geometry:
# 	$(MAKE) -C $(GEOMETRY_DIR)  # Calls `make` in the geometry directory

# # Clean rule to remove generated files
# clean:
# 	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# # Rebuild the entire project (clean + make)
# rebuild: clean all


CC = g++
CFLAGS = -I../geometry/include
LDFLAGS = -L../geometry -lmylib
TARGET = main
SRCDIR = src
OBJDIR = obj

SRC = $(SRCDIR)/main.cpp
OBJ = $(OBJDIR)/main.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

# CC = g++
# CFLAGS = -Wall -I../geometry/include
# LDFLAGS = -L../bin -lmylib

# SRCDIR = src
# OBJDIR = obj
# BINDIR = ../bin

# SRC = $(SRCDIR)/main.cpp
# OBJ = $(OBJDIR)/main.o
# TARGET = $(BINDIR)/application

# all: build-geometry $(TARGET)

# build-geometry:
# 	$(MAKE) -C ../geometry

# $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(TARGET): $(OBJ) | $(BINDIR)
# 	$(CC) $(OBJ) $(LDFLAGS) -o $@

# $(OBJDIR):
# 	mkdir -p $(OBJDIR)

# $(BINDIR):
# 	mkdir -p $(BINDIR)

# clean:
# 	rm -rf $(OBJDIR)/*.o $(TARGET)
# 	$(MAKE) -C ../geometry clean

