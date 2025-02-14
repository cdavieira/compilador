# Compilador
O projeto da disciplina de compiladores consiste em (adivinha só) construir um compilador! :)


## Etapas para construção do compilador
1. Desenvolvimento do lexer
2. Desenvolvimento do parser
3. Desenvolvimento da semântica da linguagem
4. Desenvolvimento do sistema de tipos

A etapa 1 foi feita por meio do programa `flex`, que gera código C que
implementa um autômato capaz de identificar os tokens da linguagem.

A etapa 2 foi feita por meio do programa `bison`, que é responsável por
definir a sintaxe da linguagem e implementar as regras semânticas que são
aplicáveis (etapa 3).
> o bison gera código C capaz de reconhecer linguagens livres de contexto (CFG)


## Sobre o parser

Recursos fora do escopo desse parser:
* struct
* enum
* union
* pointer
* for
* operador modulo (%)


Dicionário de termos usados no arquivo de configuração do bison (`parser/parser.y`)

| palavra        | definição       |
| -------------- | --------------- |
| declarator     | declaracao de recursos que armazenam estado (variaveis e funcoes)                   |
| stmt           | comandos 'builtin' da linguagem (if, while, ...) + chamada de funcoes               |
| block          | o bloco de definição de uma função, que pode ter stmts e declarators para variaveis |


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
