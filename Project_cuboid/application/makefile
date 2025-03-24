CC = g++
CFLAGS = -I../geometry/include
LDFLAGS = -L../bin -lmylib -Wl,-rpath,../bin  # Set runtime library path
TARGET = ../bin/main
SRCDIR = src
OBJDIR = obj

SRC = $(SRCDIR)/main.cpp
OBJ = $(OBJDIR)/main.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	mkdir -p ../bin
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJDIR) $(TARGET)
