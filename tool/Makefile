CXX = gcc
CXXFLAGS = -Wall -g
LDFLAGS = -lm -ltcl -lreadline -lcudd -lutil
SRC_DIRS = structs parser tcl CUDD_impl convert_infix signals bdd_annotation probabilities_annotations
SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = cad_tool

# Main rule
all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES) $(FLEX_DEPS) $(BISON_DEPS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
