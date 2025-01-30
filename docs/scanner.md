# Context free languages (CFL) e Context free grammars (CFG)
Context free languages são geradas por Context free grammars

uma CFG é formada por instruções que tem o seguinte formato:
```
cabeça -> corpo
```

a esse tipo de instrução se da o nome de **regra**

a cabeça corresponde a 1 simbolo que pode ser reescrito pelos simbolos que
compoem o corpo

os simbolos que compoem a cabeça e o corpo sao chamados tokens

os tokens podem ser de 2 tipos:
1. terminais
2. não-terminais

os tokens terminais são as unidades mais básicas da gramática e são
indivisíveis, isto é, não são formados por partes menores

os tokens não-terminais são aqueles que podem ser reescritos utilizando outros
tokens, sejam esses terminais e não-terminais

apenas tokens não-terminais podem aparecer na cabeça

tanto tokens terminais como não-terminais podem aparecer no corpo

* T = conjunto de tokens terminais
* N = conjunto de tokens nao terminais
* S = simbolo inicial (cabeça da primeira regra como simbolo inicial)

`derivação`: processo de reescrita de nós não terminais

uma derivação pode envolver inúmeros passos até que se chegue a algo que não é
mais possível realizar operações de substituição

o produto obtido pelos passos intermediarios se chama forma sentencial

da mesma forma, é possivel dizer que passos intermediarios geram formas
sentenciais

quando a forma sentencial só tem tokens terminais, então chegou ao fim da
derivação e chegou-se a uma sentença.



A linguagem (Context Free) é gerada a partir de um context free grammar:

G = Context Free Grammar

L(G) = Linguagem context free gerada pela aplicação de um Context Free Grammar

p = sentença

T* = conjunto formado por um numero infinito de combinações de elementos de T

S =>* p, chega-se a uma sentença por meio de um número qualquer de aplicações
de derivações partindo do simbolo inicial S



----

(1) no processo de derivação, quando se tem mais de um nó não terminal, como
decidir qual derivar primeiro?

(2) caso existam multiplas regras para derivar um nó, como decidir qual delas
utilizar primeiro?

BACKTRACKING = processo de desfazer passos de derivação / formas sentenciais
por conta de elas não terem levado a uma sentença

parsing oracular = hack que nos humanos aplicamos para evitar backtracking,
mas que é dificil de se simular em maquinas

predictive parsing => utilização do token lookahead
o parsing preditivo parte da ideia que é possível escolher uma dentre várias
possíveis regras aplicáveis a um nó não terminal a partir de 2 informações:
o estado atual da fita e o próximo token (lookahead)

(1) pergunta respondida pela orientação com que se avalia uma gramática (LEFT
TO RIGHT OU RIGHT TO LEFT)

na prática os parsers não trabalham com derivações diretamente aplicadas na
entrada, visto que um parser só precisa dizer se uma dada entrada pertence ou
nao a linguagem gerada a partir de uma gramática livre de contexto. Nesse
sentido, o parser ao produzir etapas intermediarias de derivação faz um
trabalho desnecessário do ponto de vista da geração de saída, visto que
existem diferentes formas de se chegar a um mesmo resultado ao aplicar
derivações diretas.

na prática, utiliza-se parse trees/arvore de sintaxe concreta. Essas arvores
tem uma estrutura que invaria segundo a orientação de processamento da
entrada (derivações), mas existem diferentes estruturas que sao capazes de ler
uma entrada e gerar o mesmo resultado (AMBIGUIDADE = PROBLEMAO)

níveis de prioridade é uma forma de eliminar ambiguidades

ast = arvore de sintaxe abstrata = forma simplificada da cst
essa arvore abstrai os nos intermediarios/derivacoes intermediarias que fazem
parte do parser. Portanto, servem como bons candidatos para entrada de
parsers!

ou seja, parser diferentes podem ter implementações diferentes (produzir
derivacoes de forma diferente) e ao mesmo tempo reconhecerem a mesma
linguagem, ao receberem como entrada uma AST

> linguagens equivalentes sao gramaticas que possuem a mesma linguagem

ou seja, uma AST pode representar diferentes CSTs!

====

gramatica é formada por regras de aplicação de regras em nos terminais e nao
terminais

na forma mais primal possivel: a partir da gramatica se gera a CST e a partir
da CST se gera a AST

na pratica, a AST é utilizada como uma forma de trocar mensagens entre os
diferentes modulos do compilador

========

como construir a CST?

2 estrategias: top-down e bottom-up

TOP-DOWN:
EBNF
bison: exige BNF
normalmente eh utilizado para linguagens mais simples pois oferece mais
controle

parsingLLK
automato de pilha usado para reconhecimento de grmaaticas livres de contexto

parei aqui 14:00m

========

## references
* [bison manual - GNU Bison Manual](https://www.gnu.org/software/bison/manual/bison.html)
* [how to solte 'yyerror compilation error' - Stackoverflow](https://stackoverflow.com/questions/17127481/undefined-reference-to-yyerror-when-compiling-with-flex-and-bison)
* <https://en.wikipedia.org/wiki/Shift-reduce_parser>
* info flex, Ch6
* <https://www.gnu.org/software/bison/manual/html_node/Shift_002fReduce.html>
