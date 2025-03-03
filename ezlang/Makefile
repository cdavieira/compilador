all: bison flex gcc
	@echo "Done."

bison: src/parser.y
	bison  -o src/parser.c -Wcounterexamples --header=lib/parser.h --debug $<

flex: src/scanner.l
	flex -o src/scanner.c --header-file=lib/scanner.h $<

gcc: src/scanner.c src/parser.c $(wildcard src/*.c)
	gcc -Wall -Ilib $^

clean:
	@rm -f *.o *.output src/scanner.c lib/parser.h src/parser.c a.out
