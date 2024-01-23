CXX = gcc
CXXFLAGS = -Wall -g
LDFLAGS = -lm -ltcl -lreadline -lcudd
SRC_DIRS = structs parser tcl CUDD_impl convert_infix signals flex_bison
SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = cad_tool

# Flex and Bison related files
FLEX_FILES = $(wildcard flex_bison/*.l)
BISON_FILES = $(wildcard flex_bison/*.y)
FLEX_SRC = $(FLEX_FILES:.l=.c)
BISON_SRC = $(BISON_FILES:.y=.c)
BISON_HEADERS = $(BISON_SRC:.c=.h)

# Additional dependencies for flex and bison files
FLEX_DEPS = $(FLEX_SRC:.c=.o)
BISON_DEPS = $(BISON_SRC:.c=.o)

# Main rule
all: flex_bison $(TARGET)

# Compile flex and bison files
flex_bison: $(FLEX_SRC) $(BISON_SRC)

$(FLEX_SRC): $(FLEX_FILES)
	flex -o $@ $<

$(BISON_SRC): $(BISON_FILES)
	bison --defines=$(BISON_HEADERS) -o $@ $<

# Compile source files into object files
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES) $(FLEX_DEPS) $(BISON_DEPS)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET) $(FLEX_SRC) $(FLEX_DEPS) $(BISON_SRC) $(BISON_HEADERS) $(BISON_DEPS)

.PHONY: all clean flex_bison
