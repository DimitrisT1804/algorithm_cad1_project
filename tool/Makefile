CXX = gcc
CXXFLAGS = -Wall -g 
LDFLAGS = -lm -ltcl -lreadline -lcudd -lutil -lpthread
SRC_DIRS = structs parser tcl CUDD_impl convert_infix signals bdd_annotation probabilities_annotations gui
SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
GTKFLAGS=`pkg-config --cflags gtk+-2.0`
GTTHREADFLAGS=`pkg-config --cflags gthread-2.0`
LIBS=`pkg-config --libs gtk+-2.0` `pkg-config --libs gthread-2.0` -lm
TARGET = cad_tool

# Main rule
all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) $(GTKFLAGS) $(GTTHREADFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES) $(FLEX_DEPS) $(BISON_DEPS)
	$(CXX)  $^ -o $@ $(LDFLAGS) $(GTKFLAGS) $(GTTHREADFLAGS) $(LIBS)

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

install:
	cp cad_tool /usr/local/bin


.PHONY: all clean
