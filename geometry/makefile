CC = g++
CFLAGS = -Iinclude -fPIC
LDFLAGS = -shared
TARGET = ../bin/libmylib.so  # Store output in bin/
SRCDIR = src
OBJDIR = obj

SRC = $(SRCDIR)/mylib.cpp
OBJ = $(OBJDIR)/mylib.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	mkdir -p ../bin
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -rf $(OBJDIR) $(TARGET)