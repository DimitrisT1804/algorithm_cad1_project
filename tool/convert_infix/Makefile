CXX = gcc
CXXFLAGS = -Wall -g
LDFLAGS =
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = infix_parser

# Main rule
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
