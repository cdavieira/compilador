flex_src := $(srcdir)/src/lexer.yy.c
flex_hdr := $(srcdir)/lib/lexer.yy.h
flex_files := $(flex_src) $(flex_hdr)

FLEX := flex

FLEXFLAGS :=

FLEXFLAGS += --header-file=$(flex_hdr)

FLEXFLAGS += --noline
