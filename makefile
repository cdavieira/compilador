srcdir := .

parsername := myparser
lexername := mylexer

lfile := $(srcdir)/src/lexer.l
yfile := $(srcdir)/src/parser.y

src := $(wildcard $(srcdir)/src/*/*.c)
lib := $(wildcard $(srcdir)/lib/*/*.h)

testcase := tests
testfile := tests/OK/ast/block1.c

##############################################

.PHONY: test $(parsername) $(lexername)

.SUFFIXES:

CC := gcc
CFLAGS :=
CFLAGS := -Wall -I$(srcdir)/lib
# -ll: linkar com o shared object associado ao flex (opcional)
# CFLAGS += -ll
# -ly: linkar com o shared object associado ao bison (opcional)
# CFLAGS += -ly
# Add debug information for when the parser walks in/walks out of a scope
# CFLAGS += -DDEBUG_SCOPE
# Print all the scopes and functions detected by the parser
# CFLAGS += -DDEBUG_PARSER
# Add debug information about the resulting type of each expression 
# CFLAGS += -DDEBUG_EXPR_TYPE


flex_src := $(srcdir)/src/lexer.yy.c
flex_hdr := $(srcdir)/lib/lexer.yy.h
flex_files := $(flex_src) $(flex_hdr)
FLEX := flex
FLEXFLAGS :=
FLEXFLAGS += --header-file=$(flex_hdr)
FLEXFLAGS += --noline

bison_src := $(srcdir)/src/parser.tab.c
bison_hdr := $(srcdir)/lib/parser.tab.h
bison_log := $(srcdir)/src/parser.log
bison_files := $(bison_src) $(bison_hdr) $(bison_log)
BISON := bison
BISONFLAGS := 
# --header: generate a header file
BISONFLAGS += --header=$(bison_hdr)
# --no-lines: don't use the #line preprocessor macro in the c output file
BISONFLAGS += --no-lines
# --token-table: generate a token table (whatever that is lmao)
BISONFLAGS += --token-table
# --report all: create a .output file with a report of all the errors
# BISONFLAGS += --report all
# --debug: enable tracing
BISONFLAGS += --debug
# -Wprecedence: enable warning related to precedence errors/no effect
BISONFLAGS += -Wprecedence
# -Wcounterexamples: enable examples which explain the errors
BISONFLAGS += -Wcounterexamples
BISONFLAGS += -Wconflicts-sr

VALGRIND := valgrind
VALGRINDFLAGS := --leak-check=full --show-leak-kinds=all -s

DOT := dot

LCC := lcc
LCCFLAGS := -relocation-model=pic

all: prologue $(parsername) $(lexername)

prologue:
	@echo "bison input file: $(yfile)"
	@echo "flex  input file: $(lfile)"

$(parsername): $(bison_src) $(flex_src) $(src) $(srcdir)/src/parser.c
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Parser executable: $(srcdir)/$@"

$(lexername): $(bison_src) $(flex_src) $(src) $(srcdir)/src/lexer.c
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Lexer executable: $(srcdir)/$@"

$(bison_src): $(yfile)
	$(BISON) $(BISONFLAGS) -o $@ $<

$(flex_src): $(lfile)
	$(FLEX) $(FLEXFLAGS) -o $@ $<

run:
	./$(parsername) <$(testfile) 2>$(bison_log)
	@echo "Output saved to: $(bison_log)"

pdf: clean all
	./$(parsername) $(DOT) <$(testfile)
	$(DOT) -Tpdf tmp.dot > $(notdir $(testfile)).pdf
	rm tmp.dot

# clang -S -emit-llvm main.c
ll: clean all
	./$(parsername) ll <$(testfile)
	cp tmp.ll $(notdir $(testfile)).ll
	rm tmp.ll
	$(LLC) $(LCCFLAGS) $(notdir $(testfile)).ll
	$(CC) $(notdir $(testfile)).s

val:
	@$(VALGRIND) $(VALGRINDFLAGS) ./$(parsername) < $(testfile)


test: clean all
	@./test.sh -p./$(parsername) $(testcase)

clean:
	@rm -f $(parsername)
	@rm -f $(lexername)
	@rm -f $(flex_files)
	@rm -f $(bison_files)
	@rm -f $(bison_log)
	@rm -f $(wildcard *.ll) $(wildcard *.pdf) $(wildcard *.s) a.out

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
	@echo "pdf:    generate the AST in pdf format"
	@echo "ll:     generate LLVM IR code for the AST"
	@echo ""
	@echo "Test targets"
	@echo "run:    run the parser connected to stdin and save stdout and stderr to $(bison_log)"
	@echo "val:    run the parser with a testfile through valgrind"
	@echo "test:   run the parser with all test suite"
	@echo ""
	@echo "Clean targets"
	@echo "clean:  remove all executables and intermediate files"
	@echo ""
	@echo "Makefile internal targets"
	@echo "prologue:"
