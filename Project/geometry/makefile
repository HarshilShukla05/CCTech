# # Compiler and flags
# CXX = g++
# CXXFLAGS = -Wall -fPIC -I./include  # Ensure -fPIC is applied here

# # Directories
# SRC_DIR = src
# OBJ_DIR = obj
# INCLUDE_DIR = include

# # Output shared library
# TARGET = ./src/libmylib.so

# # Find all the source files in the src directory
# SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# # Replace .cpp with .o to create object files
# OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# # Default rule to build the shared library
# # all: $(TARGET)

# # Rule to link object files into a shared library (.so)
# $(TARGET): $(OBJS)
# 	$(CXX) -shared -o $@ $(OBJS)

# # Rule to compile each source file into an object file
# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
# 	@mkdir -p $(OBJ_DIR)  # Create obj directory if it doesn't exist
# 	$(CXX) -c $(CXXFLAGS) $< -o $@

# # Clean rule to remove generated files
# clean:
# 	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# # Rebuild the entire project (clean + make)
# rebuild: clean all


CC = g++
CFLAGS = -fPIC -Iinclude
LDFLAGS = -shared
TARGET = libmylib.so
SRCDIR = src
OBJDIR = obj
INCLUDEDIR = include

SRC = $(SRCDIR)/mylib.cpp
OBJ = $(OBJDIR)/mylib.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJDIR) $(TARGET)20


# CC = g++
# CFLAGS = -fPIC -Wall -Iinclude
# LDFLAGS = -shared

# SRCDIR = src
# OBJDIR = obj
# BINDIR = ../bin

# SRC = $(SRCDIR)/mylib.cpp
# OBJ = $(OBJDIR)/mylib.o
# TARGET = $(BINDIR)/libmylib.so

# all: $(TARGET)

# $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(TARGET): $(OBJ) | $(BINDIR)
# 	$(CC) $(LDFLAGS) -o $@ $^

# $(OBJDIR):
# 	mkdir -p $(OBJDIR)

# $(BINDIR):
# 	mkdir -p $(BINDIR)

# clean:
# 	rm -rf $(OBJDIR)/*.o $(TARGET)
