#!/bin/bash

DATA=$(pwd)
IN=$DATA/in
OUT=$DATA/out

EXE=./lab06

make
for infile in `ls $IN/*.ezl`; do
    base=$(basename $infile)
    outfile=$OUT/${base/.ezl/.out}
    dotfile=$OUT/${base/.ezl/.dot}
    echo Running $base
    $EXE < $infile > $outfile 2> $dotfile
    dot -Tpdf $dotfile > $dotfile.pdf
done
