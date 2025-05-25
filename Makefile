CC = gcc
CFLAGS = -g -w
TEST_DIR = tests


# File names
LEX_FILE = lexer.l
YACC_FILE = parser.y
OUTPUT = parser

# Object files
OBJECTS = lex.yy.o parser.tab.o ast.o interpreter.o

# Default target: build the parser
all: clean
	bison -d -Wnone $(YACC_FILE) 
	flex -w $(LEX_FILE)
	@$(CC) $(CFLAGS) lex.yy.c parser.tab.c source/ast.c source/interpreter.c -o $(OUTPUT)

clean:
	@echo "Cleaning up..."
	rm -f lex.yy.c parser.tab.c parser.tab.h lex.yy.o parser.tab.o ast.o interpreter.o $(OUTPUT) 

test: all
	@echo "Running tests in $(TEST_DIR)..."
	@find $(TEST_DIR) -type f | while read file; do \
		echo "";\
		echo "Running $$file..."; \
		./$(OUTPUT) -i $$file; \
	done
# Run the program (optional)
run: $(OUTPUT)
	./$(OUTPUT)