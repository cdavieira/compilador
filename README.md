# Compilador
O projeto da disciplina de compiladores consiste em (adivinha só) construir um compilador! :)


## Etapas para construção do compilador
1. Desenvolvimento do lexer/scanner
2. Desenvolvimento do parser
3. Desenvolvimento da semântica da linguagem
4. Desenvolvimento do sistema de tipos
5. Geração da AST (Abstract Syntax Tree)
6. Desenvolvimento de um interpretador (in-memory) da AST
7. Geração de código IR para LLVM

A etapa 1 foi feita por meio do programa `flex`, que gera código C que
implementa um autômato capaz de identificar os tokens da linguagem.

As demais etapas foram feitas por meio do programa `bison`, que é responsável
por definir a sintaxe da linguagem e implementar as regras semânticas que são
aplicáveis
> o bison gera código C capaz de reconhecer linguagens livres de contexto (CFG)


## Sobre o parser
Recursos fora do escopo desse parser:
* struct
* enum
* union
* pointer
* for


Dicionário de termos usados no arquivo de configuração do bison (`parser/parser.y`)

| palavra        | definição       |
| -------------- | --------------- |
| declaration_var| linha unica do programa onde se declara 1 ou mais variaveis. A linha começa com o tipo das variaveis e continua com o **nome das variaveis** ou **declarators** |
| declarator     | nome de uma variavel |
| stmt           | comandos 'builtin' da linguagem (if, while, ...) + chamada de funcoes               |
| block          | o bloco de definição de uma função, que pode ter stmts e declarators para variaveis |


## Como rodar
```makefile
# gerar os programas do lexer e do parser (./mylexer e ./myparser)
make

# rodar todos os testes do parser
make test testcase=tests/

# rodar apenas os testes do parser que se espere que passem
make test testcase=tests/OK

# gerar um pdf da AST construida pelo parser para algum arquivo de teste
make pdf testfile=tests/OK/syntax/func/func-rec.c

# gerar o arquivo .ll (IR da LLVM) correspondente a AST
make ll testfile=tests/OK/syntax/func/func-rec.c

# testar o parser para uma entrada qualquer
./myparser < test.c
# ou
make run testfile=test.c

# limpar todos os arquivos objeto/temporarios criados
make clean
```


## Referências
* as aulas e materiais :)
* [Gramatica do C](https://www.lysator.liu.se/c/ANSI-C-grammar-y.html)
* [Gramatica de constant expressions - cppreference](https://en.cppreference.com/w/c/language/constant_expression)
* [Gramatica de expressoes - cppreference](https://en.cppreference.com/w/c/language/expressions)
* ['%define' - bison](https://www.gnu.org/software/bison/manual/html_node/_0025define-Summary.html)
* ['%code requires' - bison](https://www.gnu.org/software/bison/manual/html_node/Prologue-Alternatives.html)
* [union declaration - bison](https://www.gnu.org/software/bison/manual/html_node/Union-Decl.html)
* [token value - bison](https://www.gnu.org/software/bison/manual/html_node/Token-Values.html)
* [type generation - bison](https://www.gnu.org/software/bison/manual/html_node/Type-Generation.html)
* [lexical analysis - Wikipedia](https://en.wikipedia.org/wiki/Lexical_analysis)
* [llvm reference](https://llvm.org/docs/Reference.html#llvm-ir)
* [llvm tutorial](https://llvm.org/docs/tutorial/index.html)
* [llvm tutorial - ch3](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html)
* [llvm IR documentation - instructions](https://llvm.org/docs/LangRef.html#instruction-reference)
* [llvm IR documentation - identifiers](https://llvm.org/docs/LangRef.html#identifiers)
* [llvm IR documentation - modules](https://llvm.org/docs/LangRef.html#module-structure)
* [llvm IR documentation - globals](https://llvm.org/docs/LangRef.html#global-variables)
* [llvm IR documentation - functions](https://llvm.org/docs/LangRef.html#functions)
* [llvm IR documentation - type system](https://llvm.org/docs/LangRef.html#type-system)
* [llvm IR documentation - constants](https://llvm.org/docs/LangRef.html#constants)
* [llvm IR documentation - constant expressions](https://llvm.org/docs/LangRef.html#constant-expressions)
