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


## Referências
* as aulas e materiais :)
