#!/bin/bash

if [ -z $1 ]; then
  x=easy
else
  x=$1
fi

cp solver{.c,_c.c}
sed -i "s/###/$(sudoku -g -c$x | cut -d"|" -f1-| grep "|" |  tr " " "," | tr "." "0" | tr -d "\n" | sed "s/|,//g" | sed "s/^,//g" | sed "s/,$//")/g" solver_c.c
make
./solver
