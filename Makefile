CXX = gcc
CXXFLAGS = -Wall -g
LDFLAGS = -lm -ltcl -lreadline
SRC_DIRS = lab1 lab2
SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = cad_tool

# Main rule
all: $(TARGET)
	@echo "'$(TARGET)' is up to date. Run './$(TARGET)' to execute."

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
