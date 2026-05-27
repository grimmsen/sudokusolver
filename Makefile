COMPILER=gcc

all:
	${COMPILER} -c solver_c.c
	${COMPILER} -o solver solver_c.o
	strip solver

solver:
	${COMPILER} -c solver.c
	${COMPILER} -o solver solver.o

