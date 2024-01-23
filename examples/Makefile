# Makefile for compiling Flex and Bison files

# Compiler and flags
CC = gcc
CFLAGS = -Wall

# Flex and Bison commands
FLEX = flex
BISON = bison

# Output executable name
TARGET = parser

# Source files
LEX_SRC = lexer.l
BISON_SRC = parser.y

# Generated files
LEX_OUTPUT = lex.yy.c
BISON_OUTPUT = parser.tab.c
BISON_HEADER = parser.tab.h

# Object files
OBJ_FILES = $(LEX_OUTPUT:.c=.o) $(BISON_OUTPUT:.c=.o)

# Default target
all: $(TARGET)

# Compile the Lex file
$(LEX_OUTPUT): $(LEX_SRC)
	$(FLEX) $(LEX_SRC)

# Compile the Bison file
$(BISON_OUTPUT): $(BISON_SRC)
	$(BISON) -d $(BISON_SRC)

# Compile the main program
$(TARGET): $(LEX_OUTPUT) $(BISON_OUTPUT)
	$(CC) $(CFLAGS) -o $(TARGET) $(LEX_OUTPUT) $(BISON_OUTPUT) -lfl -lm

# Clean up intermediate files
clean:
	rm -f $(LEX_OUTPUT) $(BISON_OUTPUT) $(BISON_HEADER) $(OBJ_FILES) $(TARGET)
