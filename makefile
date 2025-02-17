export srcdir := .

parsername := myparser
lexername := mylexer

lfile := $(srcdir)/lexer/lexer.l
yfile := $(srcdir)/parser/parser.y

lookupdirs := container management type
src := $(foreach d,$(lookupdirs),$(wildcard $(srcdir)/src/$(d)/*.c))
lib := $(foreach d,$(lookupdirs),$(wildcard $(srcdir)/lib/$(d)/*.h))

testcase := tests

##############################################

.PHONY: test $(parsername) $(lexername)

all: prologue clean $(parsername) $(lexername)

include mk/cc.mk
include mk/flex.mk
include mk/bison.mk
include mk/valgrind.mk

# Add debug information for when the parser walks in/walks out of a scope
# CFLAGS += -DDEBUG_SCOPE

# Print all the scopes and functions detected by the parser
# CFLAGS += -DDEBUG_PARSER

# Print the abstract syntax tree
CFLAGS += -DDEBUG_AST

# Add debug information about the resulting type of each expression 
# CFLAGS += -DDEBUG_EXPR_TYPE

obj:
	mkdir $@

prologue:
	@echo "bison input file: $(yfile)"
	@echo "flex  input file: $(lfile)"

$(parsername): $(bison_src) $(flex_src) $(src) $(srcdir)/parser/main.c
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Parser executable: $(srcdir)/$@"

$(lexername): $(bison_src) $(flex_src) $(src) $(srcdir)/lexer/main.c
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Lexer executable: $(srcdir)/$@"

$(bison_src): $(yfile)
	$(BISON) $(BISONFLAGS) -o $@ $<

$(flex_src): $(lfile)
	$(FLEX) $(FLEXFLAGS) -o $@ $<

run:
	./$(parsername) 2> $(bison_log)
	@echo "Output saved to: $(bison_log)"

val:
	@$(VALGRIND) $(VALGRINDFLAGS) ./$(parsername) < tests/semantics/big.c

test: clean all
	@./scripts/test.sh $(testcase)

clean:
	@rm -f $(parsername)
	@rm -f $(lexername)
	@rm -f $(flex_files)
	@rm -f $(bison_files)

echo:
	@echo "src: $(src)"
	@echo "flex file: $(lfile)"
	@echo "flex files: $(flex_files)"
	@echo "flex flags: $(FLEXFLAGS)"
	@echo "bison file: $(yfile)"
	@echo "bison files: $(bison_files)"
	@echo "bison flags: $(BISONFLAGS)"

help:
	@echo "Help targets"
	@echo "help:   display this message"
	@echo "echo:   display a summary of some internal variables"
	@echo ""
	@echo "Compilation targets"
	@echo "all:    build the parser and lexer executables"
	@echo "parser: build the parser executable"
	@echo ""
	@echo "Test targets"
	@echo "run:    run the parser connected to stdin and save stdout and stderr to $(bison_log)"
	@echo "test:   run the parser with all test suite"
	@echo ""
	@echo "Clean targets"
	@echo "clean:  remove all executables and intermediate files"
	@echo ""
	@echo "Makefile internal targets"
	@echo "prologue:"
