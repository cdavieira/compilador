CC := gcc

CFLAGS :=

CFLAGS := -Wall -I$(srcdir)/lib

# -ll: linkar com o shared object associado ao flex (opcional)
# CFLAGS += -ll

# -ly: linkar com o shared object associado ao bison (opcional)
# CFLAGS += -ly
