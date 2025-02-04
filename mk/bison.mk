bison_src := $(srcdir)/src/parser.tab.c
bison_hdr := $(srcdir)/lib/parser.tab.h
bison_log := $(srcdir)/parser/parser.log
bison_files := $(bison_src) $(bison_hdr) $(bison_log)

BISON := bison

BISONFLAGS := 

# --header: generate a header file
# BISONFLAGS += --header=$(bison_hdr)

# --no-lines: don't use the #line preprocessor macro in the c output file
BISONFLAGS += --no-lines

# --token-table: generate a token table (whatever that is lmao)
BISONFLAGS += --token-table

# --report all: create a .output file with a report of all the errors
# BISONFLAGS += --report all

# --debug: enable tracing
BISONFLAGS += --debug

# -Wprecedence: enable warning related to precedence errors/no effect
# BISONFLAGS += -Wprecedence

# -Wcounterexamples: enable examples which explain the errors
# BISONFLAGS += -Wcounterexamples

# BISONFLAGS += -Wconflicts-sr
